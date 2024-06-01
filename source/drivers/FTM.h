/***************************************************************************//**
  @file     FTM.h
  @brief    FlexTimer driver
  @author   Olivia De Vincenti
 ******************************************************************************/

#ifndef _FTM_H_
#define _FTM_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"
#include "MK64F12.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define MAX_CHANNEL_COUNT	8
#define FTM_SAFE_MODE
// #define FTM_MANUAL
#define FTM_N	4
#define PWM_MAX_DC	4096
#define NO_TRIGGER	32
#define BUS_CLK_FREQ	50	// in MHz
#define USEC2TICKS(usec, prescaler)		(((usec) * BUS_CLK_FREQ) / (1 << (prescaler)))
#define MEDIAN(x1, x2)		(((x2) - (x1))/2 + (x1))	// Asume x2 > x1

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
enum COUNT_modes{UP_COUNT=0, UP_DOWN_COUNT=1};
enum CHANNEL_polarity{ACTIVE_HIGH=0, ACTIVE_LOW=1};
enum FTM_modes{INPUT_CAPTURE=0, OUTPUT_COMPARE=1, EDGE_PWM=2, CENTER_PWM=3, COMBINE_PWM=4, DUAL_EDGE_ONESHOT=6, DUAL_EDGE_CONTINUOUS=7};
enum EDGE_options{PORT_DISABLE=0, RISING_EDGE=1, FALLING_EDGE=2, BOTH_EDGES=3};
#define IC_options	EDGE_options
enum OC_options{TOGGLE_OUTPUT=1, CLEAR_OUTPUT=2, SET_OUTPUT=3};
enum PWM_options{HIGH_TRUE=2, LOW_TRUE=1};
#define DUAL_EDGE_options	EDGE_options
enum FTM_Input{BUS_CLK=1, FIXED_FREQ_CLK=2, EXTERNAL_CLK=3};

typedef uint8_t ftm_id;
typedef void (*FTM_manual_callback)();	// Callback externa, sí puede ser seteada por parte del cliente

typedef struct {
	uint8_t channel;
	uint8_t mode;
	uint8_t options;
	pin_t pin;
	uint8_t pin_mux;
	bool dma;
#ifndef FTM_SAFE_MODE
	bool polarity;
	uint8_t filter;				// Solo para channels 0, 1, 2 y 3
#endif
} ftm_channel_config_t;

typedef struct {
	uint8_t	prescaler;
	bool count_mode;
	bool overflow;
	uint8_t input_clock;		// Entrada del clock
	uint8_t channel_n;
	uint8_t trigger;
	uint8_t trigger_options;
	ftm_channel_config_t channel_arr[MAX_CHANNEL_COUNT];
} ftm_config_t;

typedef enum {
	FTM_PSC_x1		= 0x00,
	FTM_PSC_x2		= 0x01,
	FTM_PSC_x4		= 0x02,
	FTM_PSC_x8		= 0x03,
	FTM_PSC_x16		= 0x04,
	FTM_PSC_x32		= 0x05,
	FTM_PSC_x64		= 0x06,
	FTM_PSC_x128	= 0x07,
} ftm_prescaler_t;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
static FTM_Type* const FTM_PTRS[] = FTM_BASE_PTRS;      // Cambiar a sólo el registro CnV

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void FTM_Init(ftm_id id, ftm_config_t config);
bool IC_IsNewData(uint8_t id);
uint16_t IC_GetData(uint8_t id);
void PWM_Init(uint8_t id, uint8_t ch, uint16_t period, uint16_t dc);
void IC_Init(uint8_t id, uint16_t noise_th);
void FTM_StartTimer(uint8_t id);
void FTM_StopTimer(uint8_t id);
void FTM_SetManualCallback(uint8_t id, FTM_manual_callback callback);
// void PWM_SetDC(uint8_t id, uint16_t DC);
void PWM_SetON(uint8_t id, uint16_t ton);

/*******************************************************************************
 ******************************************************************************/

#endif // _FTM_H_
