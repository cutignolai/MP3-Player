/***************************************************************************//**
  @file     DMA.h
  @author   Ignacio Cutignola ft Micho
 ******************************************************************************/

#include "DMA.h"
#include "gpio.h"
#include "board.h"
#include "hardware.h"

void (*LoopCallback)(void);
void (*InputCapCallback)(void);
/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void DMA_EnableRequest(uint8_t channel);
void DMA_DisableRequest(uint8_t channel);
void DMA_IRQHandler(DMAn_t id);
void DMA_Error_IRQHandler(DMAn_t id);

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define DMA_PTR     		DMA0
#define DMA_SIZE(x)     	(((x)>>1)/(((x)>>4) + 1))
#define IS_POWER_OF_2(x)	(((x) & ((x) - 1)) == 0)
#define CHECK_LSB(y, z)     (((uint8_t) (~(0xFF << ((uint8_t)(y))) & ((uint32_t)(z)))) == 0)
#define DMA_GET_ERQ(x)      (1 << (x))

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef void (*funcallback_t)(void);	// Callback interna, no puede ser seteada por parte del cliente

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static funcallback_t DMA_callbacks[DMA_n];
static funcallback_t DMA_error_callbacks[DMA_n];

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void DMA_Config(DMA_config_t config){
	// Habilito el clock gating
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;

	// Habilito canal y setteo el request source con el FTM-CH0
	if (config.trigger || config.request_source >= DMA_MUX0) {
		DMAMUX->CHCFG[config.channel] |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_TRIG_MASK | DMAMUX_CHCFG_SOURCE((uint8_t)config.request_source);
	} else {
		DMAMUX->CHCFG[config.channel] |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE((uint8_t)config.request_source);
	}

	// Habilito las interrupciones
	NVIC_ClearPendingIRQ(config.channel);	// Elimino los eventos pendientes
	NVIC_EnableIRQ(config.channel);			// Habilito propiamente la interrupcion

	// Configuro el origen
	DMA_PTR->TCD[config.channel].SADDR = (uint32_t)(config.source_buffer); //List of Duties

	// Configuro el destino
	DMA_PTR->TCD[config.channel].DADDR = (uint32_t)(config.destination_buffer);  // To change FTM Duty

	// Configuro el offset para el origen y el destino
	DMA_PTR->TCD[config.channel].SOFF = config.source_offset;
	DMA_PTR->TCD[config.channel].DOFF = config.destination_offset;

	// Configuro la taza de transferencia del origen y destino
    DMA_PTR->TCD[config.channel].ATTR = DMA_ATTR_SSIZE(DMA_SIZE(config.transfer_size)) | DMA_ATTR_DSIZE(DMA_SIZE(config.transfer_size));

    // Configuro buffer circular
    // SI NO SE CONFIGURA ES PORQUE EL ADDRESS NO ESTÁ CORRECTAMENTE ALINEADO, EN ESE CASO USAR EL ATRIBUTO aligned. Ejemplo:
    // uint16 sourceBuffer[3] __attribute__((aligned(16))) = {0x1234, 0x5678, 0x9ABC};	Acá MOD=4 tal que L=2**MOD=16
	if (config.source_module != 0 && config.source_offset != 0){			// Source
		// Chequeo que el address sea válido
		if (config.source_module < 32 && CHECK_LSB(config.source_module, config.source_buffer)){
			DMA_PTR->TCD[config.channel].ATTR |= DMA_ATTR_SMOD(config.source_module);
		}
	}
	if (config.destination_module != 0 && config.destination_offset != 0){	// Destination
		// Chequeo que el address sea válido
		if (config.destination_module < 32 && CHECK_LSB(config.destination_module, config.destination_buffer)){
			DMA_PTR->TCD[config.channel].ATTR |= DMA_ATTR_DMOD(config.destination_module);
		}
	}

	// Offset que se sumará a SADDR o DADDR luego de cada minor loop
#ifdef DMA_MINOR_LOOP_OFFSET
	if (config.ml_offset_s || config.ml_offset_d){
		DMA_PTR->TCD[config.channel].NBYTES_MLOFFYES = DMA_NBYTES_MLOFFYES_SMLOE((uint8_t) config.ml_offset_s);		// Si aplicar el offset a source
		DMA_PTR->TCD[config.channel].NBYTES_MLOFFYES |= DMA_NBYTES_MLOFFYES_DMLOE((uint8_t) config.ml_offset_d);	// Si aplicar el offset a destination
		DMA_PTR->TCD[config.channel].NBYTES_MLOFFYES |= DMA_NBYTES_MLOFFYES_MLOFF(config.ml_offset);				// Offset a ser sumado luego de cada minor loop
		DMA_PTR->TCD[config.channel].NBYTES_MLOFFYES |= DMA_NBYTES_MLOFFYES_NBYTES(config.minor_loop_count);		// Numero de bytes para ser transferidos en cada minor loop
	} else {
		DMA_PTR->TCD[config.channel].NBYTES_MLNO = config.minor_loop_count;			// Numero de bytes para ser transferidos en cada minor loop
	}
#else
	// Numero de bytes para ser transferidos en cada minor loop
	DMA_PTR->TCD[config.channel].NBYTES_MLNO = config.minor_loop_count;
#endif

#ifdef DMA_MINOR_LINK
    // Permite pasar a otro canal al finalizar un minor loop
    if (config.link){
    	DMA_PTR->TCD[config.channel].CITER_ELINKYES = DMA_CITER_ELINKYES_ELINK(config.minor_link);
    	DMA_PTR->TCD[config.channel].CITER_ELINKYES |= DMA_CITER_ELINKYES_LINKCH(config.minor_link_ch);
    	DMA_PTR->TCD[config.channel].CITER_ELINKYES |= DMA_CITER_ELINKYES_CITER(config.mayor_loop_count);
    	DMA_PTR->TCD[config.channel].BITER_ELINKYES = DMA_PTR->TCD[config.channel].CITER_ELINKYES;
    } else {
    	DMA_PTR->TCD[config.channel].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(config.mayor_loop_count);
		DMA_PTR->TCD[config.channel].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(config.mayor_loop_count);
    }
#else
	// Cantidad de mayor loops -> Define cada cuántos minor loops salta la interrupción si es que hay una
	DMA_PTR->TCD[config.channel].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(config.mayor_loop_count);
	DMA_PTR->TCD[config.channel].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(config.mayor_loop_count);
#endif

	// Offset que se aplicará a source luego de cada mayor loop -> Es SIGNADO
	DMA_PTR->TCD[config.channel].SLAST = config.mayor_loop_offset_s;

    // DLASTSGA DLAST Scatter y Gatter
	DMA_PTR->TCD[config.channel].CSR = DMA_CSR_ESG((uint8_t) config.scatter_gather);	// Habilito o no Scatter&Gather
    DMA_PTR->TCD[config.channel].DLAST_SGA = config.mayor_loop_offset_d;				// Offset o address del próximo TCD

#ifdef DMA_MAYOR_LINK
    // Permite pasar a otro canal al finalizar un mayor loop
    DMA_PTR->TCD[config.channel].CSR |= DMA_CSR_MAJORELINK((uint8_t) config.mayor_link);
    DMA_PTR->TCD[config.channel].CSR |= DMA_CSR_MAJORELINKCH(config.mayor_link_ch);
#endif

#ifdef DMA_DEBUG
    gpioMode(DMA_TEST_PIN, OUTPUT);
#endif

	DMA_EnableRequest(config.channel);
}

void DMA_SetCallback(DMAn_t channel, void(*funcallback)(void)){
	DMA_DisableRequest(channel);
	// Habilitar interrupción luego de cada mayor loop
    DMA_PTR->TCD[channel].CSR |= DMA_CSR_INTMAJOR_MASK;
	DMA_callbacks[channel] = funcallback;
	DMA_EnableRequest(channel);
}

void DMA_SetErrorCallback(DMAn_t channel, void(*funcallback)(void)){
	DMA_DisableRequest(channel);
	DMA_error_callbacks[channel] = funcallback;
	DMA_EnableRequest(channel);
}

// Sacado del INDU que lo que hace es tomar la cuenta de los bytes que se van transfiriendo
// https://github.com/vinodstanur/frdmk64f_mp3_player/blob/master/drivers/fsl_edma.c
uint32_t DMA_GetRemainingMajorLoopCount(uint32_t channel){
    uint32_t remainingCount = 0;

    if (DMA_CSR_DONE_MASK & DMA_PTR->TCD[channel].CSR){
        remainingCount = 0;
    }
    else{
        /* Calculate the unfinished bytes */
        if (DMA_PTR->TCD[channel].CITER_ELINKNO & DMA_CITER_ELINKNO_ELINK_MASK){
            remainingCount = (DMA_PTR->TCD[channel].CITER_ELINKYES & DMA_CITER_ELINKYES_CITER_MASK) >> DMA_CITER_ELINKYES_CITER_SHIFT;
        }
        else{
            remainingCount = (DMA_PTR->TCD[channel].CITER_ELINKNO & DMA_CITER_ELINKNO_CITER_MASK) >> DMA_CITER_ELINKNO_CITER_SHIFT;
        }
    }

    return remainingCount;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void DMA_EnableRequest(uint8_t channel){
	DMA_PTR->ERQ |= DMA_GET_ERQ(channel);
	DMAMUX->CHCFG[channel] |= DMAMUX_CHCFG_ENBL_MASK;
}

void DMA_DisableRequest(uint8_t channel){
	DMA_PTR->ERQ &= ~DMA_GET_ERQ(channel);
	DMAMUX->CHCFG[channel] &= ~(DMAMUX_CHCFG_ENBL_MASK);
}

__ISR__ DMA0_IRQHandler(){
#ifdef DMA_DEBUG
	gpioWrite(DMA_TEST_PIN, 1);
#endif
	/* Clear the interrupt flag. */
	NVIC_ClearPendingIRQ(0);			// IRQn_Type: DMA#_IRQn coincide con el nro de id
	DMA_PTR->CINT |= DMA_CINT_CINT(0);
	if(DMA_callbacks[0]){
		(*DMA_callbacks[0])();
	}
#ifdef DMA_DEBUG
	gpioWrite(DMA_TEST_PIN, 0);
#endif
}

__ISR__ DMA1_IRQHandler(void){
#ifdef DMA_DEBUG
	gpioWrite(DMA_TEST_PIN, 1);
#endif
	/* Clear the interrupt flag. */
	NVIC_ClearPendingIRQ(1);			// IRQn_Type: DMA#_IRQn coincide con el nro de id
	DMA_PTR->CINT |= DMA_CINT_CINT(1);
	if(DMA_callbacks[1]){
		(*DMA_callbacks[1])();
	}
#ifdef DMA_DEBUG
	gpioWrite(DMA_TEST_PIN, 0);
#endif
}

__ISR__ DMA2_IRQHandler(void){
#ifdef DMA_DEBUG
	gpioWrite(DMA_TEST_PIN, 1);
#endif
	/* Clear the interrupt flag. */
	NVIC_ClearPendingIRQ(2);			// IRQn_Type: DMA#_IRQn coincide con el nro de id
	DMA_PTR->CINT |= DMA_CINT_CINT(2);
	if(DMA_callbacks[2]){
		(*DMA_callbacks[2])();
	}
#ifdef DMA_DEBUG
	gpioWrite(DMA_TEST_PIN, 0);
#endif
}

void DMA_Error_IRQHandler(DMAn_t id){
	/* Clear all error indicators.*/
	DMA_PTR->CERR = DMA_CERR_CAEI(1);
	if(DMA_error_callbacks[id]){
		DMA_error_callbacks[id]();
	}
}

/*******************************************************************************
 ******************************************************************************/
