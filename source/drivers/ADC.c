// /***************************************************************************//**
//   @file     ADC.c
//   @brief    ADC Driver for K64F.
//   @author   Micho
//  ******************************************************************************/

// #include <stdint.h>
// #include <stdbool.h>
// #include <stdlib.h>
// #include "MK64F12.h"
// #include "hardware.h"
// #include "ADC.h"
// #include "gpio.h"
// #include "board.h"

// /*******************************************************************************
//  * 					CONSTANT AND MACRO DEFINITIONS USING #DEFINE
//  ******************************************************************************/
// #define NUMBER_ADC 2
// #define TWO_POW_NUM_OF_CAL (1 << 4)

// /***********************************************************************************************************
//  * 									ENUMERATIONS AND STRUCTURES AND TYPEDEFS
//  ***********************************************************************************************************/

// /***********************************************************************************************************
//  * 									VARIABLE PROTOTYPES WITH GLOBAL SCOPE
//  ***********************************************************************************************************/



// /***********************************************************************************************************
//  * 													VARIABLES
//  ***********************************************************************************************************/

// static bool ADC_interrupt[2] = {false, false};
// static ADCData_t current_value[NUMBER_ADC] = {0, 0};
// static ADCChannel_t channels[NUMBER_ADC]= {0, 0};
// static bool adc_init = false;
// static ADC_t ptr_ADC = ADC0;
// static SIM_t ptr_SIM = SIM;
// static ADC_callback_t ADC_callbacks[NUMBER_ADC];

// /***********************************************************************************************************
//  * 									FUNCTION PROTOTYPES WITH GLOBAL SCOPE
//  ***********************************************************************************************************/

// /*PAG 847 REFERENCE MANUAL*/

// void ADC_Init (ADC_cfg_t config)
// {
    
// 	if (adc_init){
//         return;
//     }
// 	adc_init = true;	


//     //Clock Gaiting
//     if(config.adc == ADC0_t){                   //El micro tiene dos ADC
//         ptr_ADC = ADC0;
//         ADC0_Clock_Gating(ptr_SIM);
//         //Enable Interrups
//         NVIC_ClearPendingIRQ(ADC0_IRQn);
//         NVIC_EnableIRQ(ADC0_IRQn);
//     }
//     else{
//         ptr_ADC = ADC1;
//         ADC1_Clock_Gating(ptr_SIM, config.clocky);
//         //Enable Interrups
//         NVIC_ClearPendingIRQ(ADC1_IRQn);
//         NVIC_EnableIRQ(ADC1_IRQn);
//     }

//     ADC_SetClock(ptr_ADC, config.clocky);
//     ADC_SetResolution(config.adc, config.resolution);
// 	ADC_SetCycles(config.adc, config.cycles);
// 	ADC_Calibrate(config.adc);
//     ADC_MUXSelect(ptr_ADC, config.mux);
// 	channels[config.adc] = config.channel;
    
// 	//ADC0->CFG1 = ADC_CFG1_ADIV(0x00);
// 	//ADC1->CFG1 = ADC_CFG1_ADIV(0x00);
	
// }

// void ADC_SetResolution (ADC_num_t adc, ADCBits_t bits)
// {
// 	ADC_t ptr_ADC;
//     if(adc == ADC0_t){                  
//         ptr_ADC = ADC0;
//     }
//     else{
//         ptr_ADC = ADC1;
//     }
//     ptr_ADC->CFG1 = (ptr_ADC->CFG1 & ~ADC_CFG1_MODE_MASK) | ADC_CFG1_MODE(bits);
// }

// void ADC_SetCycles (ADC_num_t adc, ADCCycles_t cycles)
// {
// 	ADC_t ptr_ADC;
//     if(adc == ADC0_t){                  
//         ptr_ADC = ADC0;
//     }
//     else{
//         ptr_ADC = ADC1;
//     }
// 	if (cycles & ~ADC_CFG2_ADLSTS_MASK)
// 	{
// 		ptr_ADC->CFG1 &= ~ADC_CFG1_ADLSMP_MASK;
// 	}
// 	else
// 	{
// 		ptr_ADC->CFG1 |= ADC_CFG1_ADLSMP_MASK;
// 		ptr_ADC->CFG2 = (ptr_ADC->CFG2 & ~ADC_CFG2_ADLSTS_MASK) | ADC_CFG2_ADLSTS(cycles);
// 	}
// }

// bool ADC_Calibrate (ADC_num_t adc)
// {
// 	ADC_t ptr_ADC;
//     if(adc == ADC0_t){                   
//         ptr_ADC = ADC0;
//     }
//     else{
//         ptr_ADC = ADC1;
//     }
// 	int32_t  Offset		= 0;
// 	uint32_t Minus	[7] = {0,0,0,0,0,0,0};
// 	uint32_t Plus	[7] = {0,0,0,0,0,0,0};
// 	uint8_t  i;
// 	uint32_t scr3;

// 	/// SETUP
// 	ptr_ADC->SC1[0] = 0x1F;
// 	scr3 = ptr_ADC->SC3;
// 	ptr_ADC->SC3 &= (ADC_SC3_AVGS(0x03) | ADC_SC3_AVGE_MASK);

// 	/// INITIAL CALIBRATION
// 	ptr_ADC->SC3 &= ~ADC_SC3_CAL_MASK;
// 	ptr_ADC->SC3 |=  ADC_SC3_CAL_MASK;
// 	while (!(ptr_ADC->SC1[0] & ADC_SC1_COCO_MASK));
// 	if (ptr_ADC->SC3 & ADC_SC3_CALF_MASK)
// 	{
// 		ptr_ADC->SC3 |= ADC_SC3_CALF_MASK;
// 		return false;
// 	}
// 	ptr_ADC->PG  = (0x8000 | ((ptr_ADC->CLP0+ptr_ADC->CLP1+ptr_ADC->CLP2+ptr_ADC->CLP3+ptr_ADC->CLP4+ptr_ADC->CLPS) >> (1 + TWO_POW_NUM_OF_CAL)));
// 	ptr_ADC->MG  = (0x8000 | ((ptr_ADC->CLM0+ptr_ADC->CLM1+ptr_ADC->CLM2+ptr_ADC->CLM3+ptr_ADC->CLM4+ptr_ADC->CLMS) >> (1 + TWO_POW_NUM_OF_CAL)));

// 	// FURTHER CALIBRATIONS
// 	for (i = 0; i < TWO_POW_NUM_OF_CAL; i++)
// 	{
// 		ptr_ADC->SC3 &= ~ADC_SC3_CAL_MASK;
// 		ptr_ADC->SC3 |=  ADC_SC3_CAL_MASK;
// 		while (!(ptr_ADC->SC1[0] & ADC_SC1_COCO_MASK));
// 		if (ptr_ADC->SC3 & ADC_SC3_CALF_MASK)
// 		{
// 			ptr_ADC->SC3 |= ADC_SC3_CALF_MASK;
// 			return 1;
// 		}
// 		Offset += (short)ptr_ADC->OFS;
// 		Plus[0] += (unsigned long)ptr_ADC->CLP0;
// 		Plus[1] += (unsigned long)ptr_ADC->CLP1;
// 		Plus[2] += (unsigned long)ptr_ADC->CLP2;
// 		Plus[3] += (unsigned long)ptr_ADC->CLP3;
// 		Plus[4] += (unsigned long)ptr_ADC->CLP4;
// 		Plus[5] += (unsigned long)ptr_ADC->CLPS;
// 		Plus[6] += (unsigned long)ptr_ADC->CLPD;
// 		Minus[0] += (unsigned long)ptr_ADC->CLM0;
// 		Minus[1] += (unsigned long)ptr_ADC->CLM1;
// 		Minus[2] += (unsigned long)ptr_ADC->CLM2;
// 		Minus[3] += (unsigned long)ptr_ADC->CLM3;
// 		Minus[4] += (unsigned long)ptr_ADC->CLM4;
// 		Minus[5] += (unsigned long)ptr_ADC->CLMS;
// 		Minus[6] += (unsigned long)ptr_ADC->CLMD;
// 	}
// 	ptr_ADC->OFS = (Offset >> TWO_POW_NUM_OF_CAL);
// 	ptr_ADC->PG  = (0x8000 | ((Plus[0] +Plus[1] +Plus[2] +Plus[3] +Plus[4] +Plus[5] ) >> (1 + TWO_POW_NUM_OF_CAL)));
// 	ptr_ADC->MG  = (0x8000 | ((Minus[0]+Minus[1]+Minus[2]+Minus[3]+Minus[4]+Minus[5]) >> (1 + TWO_POW_NUM_OF_CAL)));
// 	ptr_ADC->CLP0 = (Plus[0] >> TWO_POW_NUM_OF_CAL);
// 	ptr_ADC->CLP1 = (Plus[1] >> TWO_POW_NUM_OF_CAL);
// 	ptr_ADC->CLP2 = (Plus[2] >> TWO_POW_NUM_OF_CAL);
// 	ptr_ADC->CLP3 = (Plus[3] >> TWO_POW_NUM_OF_CAL);
// 	ptr_ADC->CLP4 = (Plus[4] >> TWO_POW_NUM_OF_CAL);
// 	ptr_ADC->CLPS = (Plus[5] >> TWO_POW_NUM_OF_CAL);
// 	ptr_ADC->CLPD = (Plus[6] >> TWO_POW_NUM_OF_CAL);
// 	ptr_ADC->CLM0 = (Minus[0] >> TWO_POW_NUM_OF_CAL);
// 	ptr_ADC->CLM1 = (Minus[1] >> TWO_POW_NUM_OF_CAL);
// 	ptr_ADC->CLM2 = (Minus[2] >> TWO_POW_NUM_OF_CAL);
// 	ptr_ADC->CLM3 = (Minus[3] >> TWO_POW_NUM_OF_CAL);
// 	ptr_ADC->CLM4 = (Minus[4] >> TWO_POW_NUM_OF_CAL);
// 	ptr_ADC->CLMS = (Minus[5] >> TWO_POW_NUM_OF_CAL);
// 	ptr_ADC->CLMD = (Minus[6] >> TWO_POW_NUM_OF_CAL);

// 	/// UN-SETUP
// 	ptr_ADC->SC3 = scr3;

// 	return true;
// }



// void ADC_SetInterruptMode (ADC_num_t adc, bool mode)
// {
//     if(adc == ADC0_t){           
//         ADC_interrupt[0] = mode;
//     }
//     else{
//         ADC_interrupt[1] = mode;
//     }	
// }


// bool ADC_IsInterruptPending (ADC_num_t adc)
// {
// 	ADC_t ptr_ADC;
//     if(adc == ADC0_t){              
//         ptr_ADC = ADC0;
//     }
//     else{
//         ptr_ADC = ADC1;
//     }
// 	return ptr_ADC->SC1[0] & ADC_SC1_COCO_MASK;
// }

// void ADC_ClearInterruptFlag (ADC_num_t adc)
// {
// 	ADC_t ptr_ADC;
//     if(adc == ADC0_t){                 
//         ptr_ADC = ADC0;
//     }
//     else{
//         ptr_ADC = ADC1;
//     }
// 	ptr_ADC->SC1[0] = 0x00;
// }



// ADCBits_t ADC_GetResolution (ADC_num_t adc)
// {
// 	ADC_t ptr_ADC;
//     if(adc == ADC0_t){                  
//         ptr_ADC = ADC0;
//     }
//     else{
//         ptr_ADC = ADC1;
//     }
// 	return ptr_ADC->CFG1 & ADC_CFG1_MODE_MASK;
// }



// ADCCycles_t ADC_GetSCycles (ADC_num_t adc)
// {
// 	ADC_t ptr_ADC;
//     if(adc == ADC0_t){                   
//         ptr_ADC = ADC0;
//     }
//     else{
//         ptr_ADC = ADC1;
//     }
// 	if (ptr_ADC->CFG1 & ADC_CFG1_ADLSMP_MASK)
// 		return ADC_c4;
// 	else
// 		return ptr_ADC->CFG2 & ADC_CFG2_ADLSTS_MASK;
// }

// void ADC_SetHardwareAverage (ADC_num_t adc, ADCTaps_t taps)
// {
// 	ADC_t ptr_ADC;
//     if(adc == ADC0_t){                  
//         ptr_ADC = ADC0;
//     }
//     else{
//         ptr_ADC = ADC1;
//     }
// 	if (taps & ~ADC_SC3_AVGS_MASK)
// 	{
// 		ptr_ADC->SC3 &= ~ADC_SC3_AVGE_MASK;
// 	}
// 	else
// 	{
// 		ptr_ADC->SC3 |= ADC_SC3_AVGE_MASK;
// 		ptr_ADC->SC3 = (ptr_ADC->SC3 & ~ADC_SC3_AVGS_MASK) | ADC_SC3_AVGS(taps);
// 	}
// }
// ADCTaps_t ADC_GetHardwareAverage (ADC_num_t adc)
// {
// 	ADC_t ptr_ADC;
//     if(adc == ADC0_t){                   
//         ptr_ADC = ADC0;
//     }
//     else{
//         ptr_ADC = ADC1;
//     }
// 	if (ptr_ADC->SC3 & ADC_SC3_AVGE_MASK)
// 		return ADC_t1;
// 	else
// 		return ptr_ADC->SC3 & ADC_SC3_AVGS_MASK;
// }

// void ADC_Start (ADC_num_t adc, ADCChannel_t channel, ADCMux_t mux)
// {
// 	ADC_t ptr_ADC;
//     if(adc == ADC0_t){                   
//         ptr_ADC = ADC0;
//     }
//     else{
//         ptr_ADC = ADC1;
//     }
// 	ptr_ADC->CFG2 = (ptr_ADC->CFG2 & ~ADC_CFG2_MUXSEL_MASK) | ADC_CFG2_MUXSEL(mux);

// 	if (adc == ADC0_t)
// 		ptr_ADC->SC1[0] = ADC_SC1_AIEN(ADC_interrupt[0]) | ADC_SC1_ADCH(channel);
// 	else if (adc == ADC1_t)
// 		ptr_ADC->SC1[0] = ADC_SC1_AIEN(ADC_interrupt[1]) | ADC_SC1_ADCH(channel);
// }

// ADCData_t ADC_getValue(ADC_num_t adc)
// {
// 	return current_value[adc];
// }

// bool ADC_IsReady (ADC_t adc)
// {
// 	return adc->SC1[0] & ADC_SC1_COCO_MASK;
// }

// ADCData_t ADC_getData (ADC_num_t adc)
// {
// 	ADC_t ptr_ADC;
//     if(adc == ADC0_t){                   
//         ptr_ADC = ADC0;
//     }
//     else{
//         ptr_ADC = ADC1;
//     }
// 	return ptr_ADC->R[0];
// }


// /************************************************************************************************************
// * 										FUNCTION PROTOTYPES WITH LOCAL SCOPE
// ***********************************************************************************************************/
// void ADC_SetClock(ADC_t adc, ADCClock_t clocky){
//     adc->CFG1 = (adc->CFG1 & ~ADC_CFG1_ADICLK_MASK) | ADC_CFG1_ADICLK(bus_clock);       //usamos busclock
// 	adc->CFG1 = ADC_CFG1_ADIV(clocky);
// }

// void ADC0_Clock_Gating(SIM_t sim){
//     sim->SCGC6 |= SIM_SCGC6_ADC0_MASK;
// }

// void ADC1_Clock_Gating(SIM_t sim, ADCClock_t select){
//     sim->SCGC3 |= SIM_SCGC3_ADC1_MASK;
//     ADC1->CFG1 = ADC_CFG1_ADIV(select);
// }

// void ADC_MUXSelect(ADC_t adc, ADCMux_t mux){
//     adc->CFG2 = (adc->CFG2 & ~ADC_CFG2_MUXSEL_MASK) | ADC_CFG2_MUXSEL(mux);
// }




















// void ADC_SetInterruptCallback(ADC_num_t adc, ADC_callback_t callback_fn){
// 	ADC_callbacks[adc] = callback_fn;
// }

// __ISR__ ADC0_IRQHandler(void){
// 	current_value[ADC0_t] = ADC_getData(ADC0_t);

// 	if(ADC_callbacks[0]){
// 		ADC_callbacks[0]();
// 	}
// }

// __ISR__ ADC1_IRQHandler(void){
// 	current_value[ADC1_t] = ADC_getData(ADC1_t);

// 	if(ADC_callbacks[1]){
// 		ADC_callbacks[1]();
// 	}
// }
