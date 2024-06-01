// /***************************************************************************//**
//   @file     ADC.c
//   @brief    ADC Driver for K64F.
//   @author   Micho
//  ******************************************************************************/

// #ifndef _ADC_H_
// #define _ADC_H_

// /*******************************************************************************
//  * INCLUDE HEADER FILES
//  ******************************************************************************/

// #include <stdint.h>
// #include <stdbool.h>
// #include <stdlib.h>
// #include "MK64F12.h"


// /*******************************************************************************
//  * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
//  ******************************************************************************/



// /*******************************************************************************
//  * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
//  ******************************************************************************/
// typedef void (*ADC_callback_t)(void);

// typedef enum
// {
// 	ADC_b8,
// 	ADC_b12,
// 	ADC_b10,
// 	ADC_b16,
// } ADCBits_t;

// typedef enum
// {
// 	ADC_c24,
// 	ADC_c16,
// 	ADC_c10,
// 	ADC_c6,
// 	ADC_c4,
// } ADCCycles_t;

// typedef enum
// {
// 	ADC_t4,
// 	ADC_t8,
// 	ADC_t16,
// 	ADC_t32,
// 	ADC_t1,
// } ADCTaps_t;

// typedef enum
// {
// 	ADC_mA,
// 	ADC_mB,
// } ADCMux_t;

// typedef enum                                    
// {
// 	clock,
// 	clock2,	//Clock/2
// 	clock4,	//Clock/4
// 	clock8,	//Clock/8

// } ADCClock_t;

// typedef enum    
// {
// 	bus_clock
// }ADC_adiclock;

// typedef ADC_Type *ADC_t;
// typedef SIM_Type *SIM_t;
// typedef uint8_t ADCChannel_t; /* Channel 0-23 */
// typedef uint16_t ADCData_t;

// typedef enum ADC_Type
// {
// 	ADC0_t,
// 	ADC1_t,
// } ADC_num_t;



// typedef struct {
//     ADC_num_t adc;
//     ADCBits_t resolution;
//     ADCCycles_t cycles;
//     ADCClock_t clocky;
//     ADCMux_t mux;
//     ADCChannel_t channel;
// } ADC_cfg_t;




// /*******************************************************************************
//  * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
//  ******************************************************************************/

// void ADC_Init (ADC_cfg_t config);
// void ADC_SetResolution (ADC_num_t adc, ADCBits_t bits);
// void ADC_SetCycles (ADC_num_t adc, ADCCycles_t cycles);
// bool ADC_Calibrate (ADC_num_t adc);
// void ADC_SetInterruptMode (ADC_num_t adc, bool mode);
// bool ADC_IsInterruptPending (ADC_num_t adc);
// void ADC_ClearInterruptFlag (ADC_num_t adc);
// ADCBits_t ADC_GetResolution (ADC_num_t adc);
// ADCCycles_t ADC_GetSCycles (ADC_num_t adc);
// void ADC_SetHardwareAverage (ADC_num_t adc, ADCTaps_t taps);
// ADCTaps_t ADC_GetHardwareAverage (ADC_num_t adc);
// void ADC_Start (ADC_num_t adc, ADCChannel_t channel, ADCMux_t mux);
// ADCData_t ADC_getValue(ADC_num_t adc);
// bool ADC_IsReady (ADC_t adc);
// ADCData_t ADC_getData (ADC_num_t adc);


// /*******************************************************************************
//  * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
//  ******************************************************************************/

// void ADC_SetClock(ADC_t adc, ADCClock_t clocky);
// void ADC0_Clock_Gating(SIM_t sim);
// void ADC1_Clock_Gating(SIM_t sim, ADCClock_t select);
// void ADC_MUXSelect(ADC_t adc, ADCMux_t mux);


// #endif // _ADC_H_
