/***************************************************************************//**
  @file     CMP.c
  @brief    CMP Driver for K64F
  @author   Pedro DL
 ******************************************************************************/

#include "CMP.h"
#include "MK64F12.h"

/************************************************************************************************************
 * 				                	CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ***********************************************************************************************************/
#define CMP0_IN1_DEF PORTNUM2PIN(PC, 7)
#define CMP0_OUT_DEF PORTNUM2PIN(PC, 5)
#define CMP1_IN1_DEF PORTNUM2PIN(PC, 3)
#define CMP1_OUT_DEF PORTNUM2PIN(PC, 4)

#define V_REF 3.33
#define VO_SEL 1.65
#define DACO_VS (VO_SEL/(V_REF/64)-1)

/***********************************************************************************************************
 * 									ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ***********************************************************************************************************/

/***********************************************************************************************************
 * 									VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ***********************************************************************************************************/

/***********************************************************************************************************
 * 													VARIABLES
 ***********************************************************************************************************/
static CMP_Type * CMPs[] = CMP_BASE_PTRS;
static IRQn_Type IRQs[] = CMP_IRQS;
static PORT_Type* portPtrs[] = PORT_BASE_PTRS;

/***********************************************************************************************************
 * 									FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ***********************************************************************************************************/
CMP_cfg_t CMP_get_cfg_default (void){
    CMP_cfg_t config;
    config.h_lvl = h_lvl_0;
    config.high_speed = true;
    config.inverter = false;
    config.output_pin_en = true;
    config.DAC.DAC_en = true;
    config.DAC.Vref = Vin2;
    config.DAC.output_vol = DACO_VS;
    return config;
}


void CMP_init(CMP_cfg_t config, CMP_t n){

    //CLK Enable
    SIM->SCGC4 |= SIM_SCGC4_CMP_MASK;

    //CR0
    CMPs[n]->CR0 = CMP_CR0_HYSTCTR(config.h_lvl);
    CMPs[n]->CR0 |= CMP_CR0_FILTER_CNT(1);          //Comparator simply sampled. See set_filter() for more

    //CR1
    CMPs[n]->CR1 |= CMP_CR1_EN_MASK;

    if(config.output_pin_en){
        CMPs[n]->CR1 |= CMP_CR1_OPE_MASK; 
    }

    if(config.inverter){
        CMPs[n]->CR1 |= CMP_CR1_INV_MASK;
    }

    if(config.high_speed){
        CMPs[n]->CR1 |= CMP_CR1_PMODE_MASK;
    }

    //SCR
    //CMP_manage_interrupts(true, n);

    //DACCR
    if(config.DAC.DAC_en){
        CMPs[n]->DACCR = CMP_DACCR_DACEN_MASK;
        CMPs[n]->DACCR |= CMP_DACCR_VRSEL(config.DAC.Vref);
        CMPs[n]->DACCR |= CMP_DACCR_VOSEL(config.DAC.output_vol);
    }
}

void CMP_set_filter(CMP_t n, uint8_t sample_count, uint8_t period){
    if (sample_count <= 0){
        CMPs[n]->CR1 &= ~CMP_CR1_SE_MASK;       //SE must be on 0 to be a legal state
        CMPs[n]->CR0 &= ~CMP_CR0_FILTER_CNT_MASK;
        CMPs[n]->CR0 |= CMP_CR0_FILTER_CNT(0);
        CMPs[n]->FPR = CMP_FPR_FILT_PER(0); 
    }
    else if ((sample_count <= 7) && (sample_count >= 1)){
        CMPs[n]->CR0 &= ~CMP_CR0_FILTER_CNT_MASK;
        CMPs[n]->CR0 |= CMP_CR0_FILTER_CNT(sample_count);
        CMPs[n]->FPR = CMP_FPR_FILT_PER(period); 
    }
}

void CMP_manage_interrupts(bool en, CMP_t n){
    if (en){
        CMPs[n]->SCR = (CMP_SCR_IEF_MASK | CMP_SCR_IER_MASK);
        if( (CMPs[n]->SCR  & (CMP_SCR_IEF_MASK | CMP_SCR_IER_MASK)) !=0 ){
            NVIC_EnableIRQ(IRQs[n]);
        }
			
    }
    else{
        CMPs[n]->SCR &= ~(CMP_SCR_IEF_MASK | CMP_SCR_IER_MASK);
        if( (CMPs[n]->SCR  & (CMP_SCR_IEF_MASK | CMP_SCR_IER_MASK)) ==0 ){
            NVIC_DisableIRQ(IRQs[n]);
        }
    }
}

void CMP_set_input (CMP_input p_input, CMP_input m_input, CMP_t n){
    CMPs[n]->MUXCR = (CMPs[n]->MUXCR & ~CMP_MUXCR_PSEL_MASK) | CMP_MUXCR_PSEL(1);
    CMPs[n]->MUXCR = (CMPs[n]->MUXCR & ~CMP_MUXCR_MSEL_MASK) | CMP_MUXCR_MSEL(7);
}

void CMP_set_output(CMP_output output){
    if(output == FTM1_CH0){
        //SIM->SOPT4 &= ~(SIM_SOPT4_FTM1CH0SRC_MASK);
		//SIM->SOPT4 |= SIM_SOPT4_FTM1CH0SRC(1);
    }
    else if (output == FTM2_CH0){
        //SIM->SOPT4 &= ~(SIM_SOPT4_FTM2CH0SRC_MASK);
		//SIM->SOPT4 |= SIM_SOPT4_FTM2CH0SRC(1);
    }
}

uint8_t CMP_get_output(CMP_t n){
    uint8_t output = (uint8_t)(CMPs[n]->SCR & CMP_SCR_COUT_MASK);
    return output;
}

void set_port_mux(pin_t Pin, uint8_t mux)
{
	PORT_Type *port = portPtrs[PIN2PORT(Pin)];
	uint32_t actualPCR = (port->PCR)[PIN2NUM(Pin)];
	actualPCR = (actualPCR) & (~PORT_PCR_MUX_MASK); 
	actualPCR = actualPCR | PORT_PCR_MUX(mux); 
	(port->PCR)[PIN2NUM(Pin)] = actualPCR; 
}

void CMP_full_init(CMP_t n){         //La configuración usada para probarlo
    CMP_cfg_t config;
    config = CMP_get_cfg_default();

    uint8_t cmp_in_mux = 0;
	uint8_t cmp_out_mux = 6;
    set_port_mux(CMP0_IN1_DEF, cmp_in_mux);
    set_port_mux(CMP0_OUT_DEF, cmp_out_mux);

    config.h_lvl = h_lvl_3;
   
    CMP_init(config, n);
    CMP_set_input(IN1, IN7, n);
    CMP_set_output(FTM1_CH0);
    
}


/************************************************************************************************************
* 										FUNCTION PROTOTYPES WITH LOCAL SCOPE
***********************************************************************************************************/

