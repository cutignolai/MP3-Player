/***************************************************************************//**
  @file     FTM.c
  @brief    FlexTimer Driver
  @author   Olivia De Vincenti
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdio.h>
#include "MK64F12.h"
#include "hardware.h"
#include "gpio.h"
#include "FTM.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define READ_BIT(reg, bit_mask)		(((reg) & (bit_mask)) == (bit_mask))
#define BYTE	8
#define MAX_COUNT	0xFFFF
#define MAX_TRIGGER	31
#define FTM_TEST_PIN	PORTNUM2PIN(PC, 3)
//#define PWM_PIN		PORTNUM2PIN(PC, 10)

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
enum IC_states{IC_START, IC_END};
enum PWM_cycles{POS_CYCLE, NEG_CYCLE};
typedef void (*FTM_callback)(uint8_t id, uint8_t ch);	// Callback interna, no puede ser seteada por parte del cliente

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
void IC_ISR(uint8_t id, uint8_t ch);
void PWM_ISR(uint8_t id, uint8_t ch);
void set_channel(uint8_t id, ftm_channel_config_t* config);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static PORT_Type* const PORT_PTRS[] = PORT_BASE_PTRS;
//static uint16_t const PORT_SCG[] = {SIM_SCGC5_PORTA_MASK, SIM_SCGC5_PORTB_MASK, SIM_SCGC5_PORTC_MASK, SIM_SCGC5_PORTD_MASK, SIM_SCGC5_PORTE_MASK};
// static FTM_Type* const FTM_PTRS[] = FTM_BASE_PTRS;
static bool IC_state[] = {0, 0, 0, 0};
static uint16_t IC_ovf[] = {0, 0, 0, 0};
static uint16_t IC_med1[] = {0, 0, 0, 0};
static uint16_t IC_med2[] = {0, 0, 0, 0};
static uint16_t IC_med[] = {0, 0, 0, 0};
static bool IC_new_data[] = {false, false, false, false};
static uint16_t IC_noiseTH[] = {0, 0, 0, 0};
static uint16_t PWM_period[] = {0, 0, 0, 0};
// static uint16_t PWM_DC[] = {0, 0, 0, 0};
static uint16_t PWM_Ton[] = {0, 0, 0, 0};
static FTM_callback ISR_callback[FTM_N];
#ifdef FTM_MANUAL
static FTM_manual_callback ISR_manual_callback[FTM_N];
#endif
static uint8_t ISR_ch[FTM_N];
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void FTM_Init(ftm_id id, ftm_config_t config){

#ifdef FTM_DEBUG
	gpioMode(FTM_TEST_PIN, OUTPUT);
#endif

	FTM_Type* ftm = FTM_PTRS[id];
	// Clock gating
	switch (id){
		case 0:
			SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;
			NVIC_EnableIRQ(FTM0_IRQn);
			FTM0->PWMLOAD = FTM_PWMLOAD_LDOK_MASK | 0x0F;
		break;

		case 1:
			SIM->SCGC6 |= SIM_SCGC6_FTM1_MASK;
			NVIC_EnableIRQ(FTM1_IRQn);
			FTM1->PWMLOAD = FTM_PWMLOAD_LDOK_MASK | 0x0F;
		break;

		case 2:
			SIM->SCGC6 |= SIM_SCGC6_FTM2_MASK;

			SIM->SCGC3 |= SIM_SCGC3_FTM2_MASK;
			NVIC_EnableIRQ(FTM2_IRQn);
			FTM2->PWMLOAD = FTM_PWMLOAD_LDOK_MASK | 0x0F;
		break;

		case 3:
			SIM->SCGC3 |= SIM_SCGC3_FTM3_MASK;
			NVIC_EnableIRQ(FTM3_IRQn);
			FTM3->PWMLOAD = FTM_PWMLOAD_LDOK_MASK | 0x0F;
		break;
	}

	// Config pin
	PORT_PTRS[PIN2PORT(config.channel_arr[0].pin)]->PCR[PIN2NUM(config.channel_arr[0].pin)] = 0x00000000;
	PORT_PTRS[PIN2PORT(config.channel_arr[0].pin)]->PCR[PIN2NUM(config.channel_arr[0].pin)] |= PORT_PCR_MUX(config.channel_arr[0].pin_mux);
	PORT_PTRS[PIN2PORT(config.channel_arr[0].pin)]->PCR[PIN2NUM(config.channel_arr[0].pin)] |= PORT_PCR_DSE_MASK;
	PORT_PTRS[PIN2PORT(config.channel_arr[0].pin)]->PCR[PIN2NUM(config.channel_arr[0].pin)] &= ~PORT_PCR_IRQC_MASK;
	PORT_PTRS[PIN2PORT(config.channel_arr[0].pin)]->PCR[PIN2NUM(config.channel_arr[0].pin)] |= PORT_PCR_IRQC(GPIO_IRQ_MODE_DISABLE);

	// Disable write protection
	ftm->FMS |= FTM_FMS_WPEN_MASK;
	if (READ_BIT(ftm->FMS, FTM_FMS_WPEN_MASK)){
		ftm->MODE |= FTM_MODE_WPDIS_MASK;
	}

	ftm->MODE &= ~FTM_MODE_FTMEN_MASK;	// FTM Basic
	ftm->QDCTRL &= ~FTM_QDCTRL_QUADEN_MASK;

	ftm->SC = (ftm->SC & ~FTM_SC_PS_MASK) | FTM_SC_PS(config.prescaler);		// Set prescaler

	ftm->CNTIN=0x0000;					// Clock counts from 0x0000
	ftm->CNT=0x0000;					// Starts at 0x0000
	ftm->MOD=FTM_MOD_MOD(MAX_COUNT);	// To 0xFFFF

	if (config.overflow){
		ftm->SC |= FTM_SC_TOIE_MASK;	// Enable Overflow interrupt
	}

	uint8_t i;
	for (i = 0; i < config.channel_n; i++){
		set_channel(id, &config.channel_arr[i]);
	}

	if (config.trigger <= MAX_TRIGGER){
		SIM->SOPT4 |= config.trigger_options << config.trigger;
	}

	//ftm->FMS |= FTM_FMS_WPEN_MASK;	// Enable Write protection

}

void FTM_StartTimer(uint8_t id){
	FTM_PTRS[id]->SC |= FTM_SC_CLKS(1);		// Select System clock (Bus Clock)
}

void FTM_StopTimer(uint8_t id){
	FTM_PTRS[id]->SC &= ~FTM_SC_CLKS(1);	// Select System clock (Bus Clock)
}

void IC_Init(uint8_t id, uint16_t noise_th){
	IC_noiseTH[id] = noise_th;
}

void PWM_Init(uint8_t id, uint8_t ch, uint16_t period, uint16_t dc){
	PWM_period[id] = period;
	FTM_PTRS[id]->MOD = period - 1;
	FTM_PTRS[id]->CONTROLS[ch].CnV = dc * period / 100;
	// PWM_DC[id] = dc;
    PWM_Ton[id] = dc * period / 100;
}

#ifdef FTM_MANUAL
void FTM_SetManualCallback(uint8_t id, FTM_manual_callback callback){
    ISR_manual_callback[id] = callback;
}
#endif

bool IC_IsNewData(uint8_t id){
	return IC_new_data[id];
}

uint16_t IC_GetData(uint8_t id){
	IC_new_data[id] = false;
	return IC_med[id];
}

// void PWM_SetDC(uint8_t id, uint16_t DC){
// 	PWM_DC[id] = DC; //* 100 / OFFSET_TO_FTM;
// }

void PWM_SetON(uint8_t id, uint16_t ton){
    PWM_Ton[id] = ton;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void IC_ISR(uint8_t id, uint8_t ch){

	FTM_Type* ftm = FTM_PTRS[id];

	if ( READ_BIT(ftm->SC, FTM_SC_TOF_MASK) ){		// If overflow interrupt
		ftm->SC &= ~FTM_SC_TOF_MASK;		// Clear overflow interrupt flag
		if (IC_state[id] == IC_END){
			IC_ovf[id]++;
		}
	} else if ( READ_BIT(ftm->CONTROLS[ch].CnSC, FTM_CnSC_CHF_MASK) ){		// If match interrupt
		ftm->CONTROLS[ch].CnSC &= ~FTM_CnSC_CHF_MASK;		// Clear interrupt flag
		if ( IC_state[id] == IC_START ){
			IC_med1[id] = ftm->CONTROLS[ch].CnV & FTM_CnV_VAL_MASK;
			IC_state[id] = IC_END;
		} else if ( IC_state[id] == IC_END ){
			IC_med2[id] = ftm->CONTROLS[ch].CnV & FTM_CnV_VAL_MASK;
			uint32_t aux = IC_med2[id] + IC_ovf[id] * MAX_COUNT - IC_med1[id];
			if ( aux > IC_noiseTH[id]){
				IC_ovf[id] = 0;
				IC_med[id] = aux;
				IC_state[id] = IC_START;
				IC_new_data[id] = true;
			}
		}
	}
}

void PWM_ISR(uint8_t id, uint8_t ch){
    
	FTM_Type* ftm = FTM_PTRS[id];

	if ( READ_BIT(ftm->SC, FTM_SC_TOF_MASK) ){		// If overflow interrupt
		ftm->SC &= ~FTM_SC_TOF_MASK;			// Clear overflow interrupt flag
	}
	if ( READ_BIT(ftm->CONTROLS[ch].CnSC, FTM_CnSC_CHF_MASK) ){		// If match interrupt
		ftm->CONTROLS[ch].CnSC &= ~FTM_CnSC_CHF_MASK;		// Clear interrupt flag
	}
	ftm->CONTROLS[ch].CnV = FTM_CnV_VAL(PWM_Ton[id]);
#ifdef FTM_MANUAL
    (*ISR_manual_callback[id])();
#endif
}

void set_channel(uint8_t id, ftm_channel_config_t* config){

	FTM_Type* ftm = FTM_PTRS[id];

	switch (config->mode){

	case INPUT_CAPTURE:
		ftm->CONTROLS[config->channel].CnSC = (00 << FTM_CnSC_MSA_SHIFT) | (config->options << FTM_CnSC_ELSA_SHIFT);
		ISR_callback[id] = IC_ISR;
		ISR_ch[id] = config->channel;
// #ifndef FTM_SAFE_MODE
// 		ftm->COMBINE &= ~(1 << (FTM_COMBINE_COMBINE0_SHIFT * config->channel + BYTE));
// 		ftm->COMBINE &= ~(1 << (FTM_COMBINE_DECAPEN0_SHIFT * config->channel + BYTE));
// 		// if (config->channel < 4){
// 		// 	ftm->FILTER &= ~ (FTM_FILTER_CH0FVAL_MASK << (4 * config->channel));
// 		// 	ftm->FILTER |= (( config->filter & FTM_FILTER_CH0FVAL_MASK) << (4 * config->channel));
// 		// }
// #endif
		break;

	case OUTPUT_COMPARE:
		ftm->CONTROLS[config->channel].CnSC = (01 << FTM_CnSC_MSA_SHIFT) | (config->options << FTM_CnSC_ELSA_SHIFT);
// #ifndef FTM_SAFE_MODE
// 		ftm->COMBINE &= ~(1 << (FTM_COMBINE_COMBINE0_SHIFT * config->channel + BYTE));
// 		ftm->COMBINE &= ~(1 << (FTM_COMBINE_DECAPEN0_SHIFT * config->channel + BYTE));
// #endif
		break;

	case EDGE_PWM:
		ftm->CONTROLS[config->channel].CnSC = (10 << FTM_CnSC_MSA_SHIFT) | (config->options << FTM_CnSC_ELSA_SHIFT);
		ISR_callback[id] = PWM_ISR;
		ISR_ch[id] = config->channel;
// #ifndef FTM_SAFE_MODE
// 		ftm->COMBINE &= ~(1 << (FTM_COMBINE_COMBINE0_SHIFT * config->channel + BYTE));
// 		ftm->COMBINE &= ~(1 << (FTM_COMBINE_DECAPEN0_SHIFT * config->channel + BYTE));
// #endif
		break;

	case CENTER_PWM:
		ftm->SC |= FTM_SC_CPWMS_MASK;
		ftm->CONTROLS[config->channel].CnSC = config->options << FTM_CnSC_ELSA_SHIFT;
// #ifndef FTM_SAFE_MODE
// 		ftm->COMBINE &= ~(1 << (FTM_COMBINE_COMBINE0_SHIFT * config->channel + BYTE));
// 		ftm->COMBINE &= ~(1 << (FTM_COMBINE_DECAPEN0_SHIFT * config->channel + BYTE));
// #endif
		break;

	case COMBINE_PWM:
		ftm->SC &= ~FTM_SC_CPWMS_MASK;
		ftm->CONTROLS[config->channel].CnSC = config->options << FTM_CnSC_ELSA_SHIFT;
		ftm->COMBINE |= (1 << (FTM_COMBINE_COMBINE0_SHIFT * config->channel + BYTE));
		ftm->COMBINE &= ~(1 << (FTM_COMBINE_DECAPEN0_SHIFT * config->channel + BYTE));
		break;

	case DUAL_EDGE_ONESHOT:
		ftm->CONTROLS[config->channel].CnSC = (00 << FTM_CnSC_MSA_SHIFT) | (config->options << FTM_CnSC_ELSA_SHIFT);
		ftm->COMBINE &= ~(1 << (FTM_COMBINE_COMBINE0_SHIFT * config->channel + BYTE));
		ftm->COMBINE |= (1 << (FTM_COMBINE_DECAPEN0_SHIFT * config->channel + BYTE));
		break;

	case DUAL_EDGE_CONTINUOUS:
		ftm->CONTROLS[config->channel].CnSC = (01 << FTM_CnSC_MSA_SHIFT) | (config->options << FTM_CnSC_ELSA_SHIFT);
		ftm->COMBINE &= ~(1 << (FTM_COMBINE_COMBINE0_SHIFT * config->channel + BYTE));
		ftm->COMBINE |= (1 << (FTM_COMBINE_DECAPEN0_SHIFT * config->channel + BYTE));
		break;

	default:
		break;
	}

	//ftm->POL = config->polarity << config->channel;

	ftm->CONTROLS[config->channel].CnSC = (ftm->CONTROLS[config->channel].CnSC & ~FTM_CnSC_CHIE_MASK) | FTM_CnSC_CHIE(1);

	if (config->dma){
		ftm->CONTROLS[config->channel].CnSC = (ftm->CONTROLS[config->channel].CnSC & ~FTM_CnSC_DMA_MASK) | FTM_CnSC_DMA((uint8_t)config->dma);
	}

}

/******************************************************************************/

__ISR__ FTM0_IRQHandler(void){
#ifdef FTM_DEBUG
	gpioWrite(FTM_TEST_PIN, 1);
#endif
	(*ISR_callback[0])(0, ISR_ch[0]);
#ifdef FTM_DEBUG
	gpioWrite(FTM_TEST_PIN, 0);
#endif
}

__ISR__ FTM1_IRQHandler(void){
	(*ISR_callback[1])(1, ISR_ch[1]);
}

__ISR__ FTM2_IRQHandler(void){
	(*ISR_callback[2])(2, ISR_ch[2]);
}

__ISR__ FTM3_IRQHandler(void){
	(*ISR_callback[3])(3, ISR_ch[3]);
}
