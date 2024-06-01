// /***************************************************************************//**
//   @file     App.c
//   @brief    Application functions
//   @author   Nicolás Magliola
//  ******************************************************************************/

// /*******************************************************************************
//  * INCLUDE HEADER FILES
//  ******************************************************************************/
// #include <stdio.h>
// // #include <os.h>
// // #include <string.h>
// #include <drivers/board.h>
// #include <drivers/gpio.h>
// #include <drivers/FTM.h>
// #include <drivers/timer.h>
// #include <drivers/dma.h>
// // #include <drivers/leds.h>
// // #include <drivers/encoder.h>
// // #include <drivers/card_reader.h>
// // #include <drivers/button.h>
// // #include <drivers/user.h>
// // #include <drivers/message.h>
// #include "MK64F12.h"

// /*******************************************************************************
//  * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
//  ******************************************************************************/

// #define BYTE_SIZE       8
// #define PIXEL_BYTES		3
// #define MATRIX_SIZE     64

// /***** PIXEL TIMING *****/
// #define	Ts			1.25 //us
// #define T0H			0.4 //us
// #define T0L			0.85 //us
// #define T1H			0.8 //us
// #define T1L			0.45 //us
// #define RES			50 //us

// /**** OVERALL TIMING ****/
// #define REFRESH_PERIOD	16000 //us

// /******* PINS *******/
// #define DIN     PORTNUM2PIN(PC, 9)
// #define DOUT    PORTNUM2PIN(PC, 10)

// #define LED_MATRIX_PRESCALER FTM_PSC_x1

// //----------------------------------------

// //----------- COLORES DEL LED ------------
// typedef struct {
//     uint8_t green;
//     uint8_t red;
//     uint8_t blue;
// } matrix_pixel_t;

// matrix_pixel_t red = {0, 255, 0};
// matrix_pixel_t yellow = {200, 200, 0};
// matrix_pixel_t purple = {0, 255, 200};
// matrix_pixel_t orange = {128, 255, 0};
// matrix_pixel_t green = {128, 0, 0};
// matrix_pixel_t blue = {128, 0, 0};

// //-----------------------------------------
// void toggle_DC();


// /*******************************************************************************
//  * ENUMS AND STRUCTURES
//  ******************************************************************************/




// /*******************************************************************************
//  * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
//  ******************************************************************************/


// /*******************************************************************************
//  * VARIABLES
//  ******************************************************************************/
// //tim_id_t refresh_timer;
// bool bit = true;
// int16_t matrix_buffer[2048];	//sample buffer
// // int16_t matrix_buffer[PIXEL_BYTES*MATRIX_SIZE*MATRIX_SIZE];	//sample buffer


// /*******************************************************************************
//  *******************************************************************************
//                         GLOBAL FUNCTION DEFINITIONS
//  *******************************************************************************
//  ******************************************************************************/

// /* Función que se llama 1 vez, al comienzo del programa */
// void App_Init (void)
// {
//     gpioMode(DIN, OUTPUT);
// 	gpioMode(DOUT, INPUT);
//     #define FTM_ID  0
//     #define PWM_CH  1

//     DMA_config_t DMAconfig = {.channel = DMA_2, .source_buffer = matrix_buffer,
// 								 .destination_buffer = &(FTM_PTRS[FTM_ID]->CONTROLS[PWM_CH].CnV), .source_offset = sizeof(uint8_t), .transfer_size = 0,
// 								 .source_full_size = PIXEL_BYTES*MATRIX_SIZE*MATRIX_SIZE*sizeof(uint8_t), .source_unit_size = sizeof(uint8_t), .request_source = DMA_FTM0CH1};
	
// 	DMA_Config(DMAconfig);

//     // CREATE TIMERS
//     ftm_channel_config_t ch1 = { .channel = PWM_CH, .mode = EDGE_PWM, .options = HIGH_TRUE, .dma = true, .pin = PORTNUM2PIN(PC, 2), .pin_mux = 4 };
// 	ftm_config_t ftm0 = { .prescaler = LED_MATRIX_PRESCALER, .count_mode = UP_COUNT, .overflow = true, .input_clock = BUS_CLK, .trigger = NO_TRIGGER, .trigger_options = 0, .channel_n = 1, .channel_arr[0] = ch1 };
//     FTM_Init(FTM_ID, ftm0);
//     PWM_Init(FTM_ID, USEC2TICKS(Ts, LED_MATRIX_PRESCALER), 100*T1H/Ts);
// 	FTM_StartTimer(FTM_ID);
//     // refresh_timer = timerGetId();
//     // timerCreate(refresh_timer, TIMER_MS2TICKS(REFRESH_PERIOD), TIM_MODE_PERIODIC, send_nzr_bit);

//     // timerActivate(refresh_timer);
// }

// /* Función que se llama constantemente en un ciclo infinito */
// void App_Run (void)
// {
//     uint8_t i = 0;
//     for (i = 0; i < PIXEL_BYTES*MATRIX_SIZE; i++){
//         if (i % 2){
//             matrix_buffer[i] = FTM_CnV_VAL(35);
//         } else {
//             matrix_buffer[i] = FTM_CnV_VAL(60);
//         }
//     }
// 	// matrix_pixel_t pixel = red;
// 	// int i, j;
// 	// for (i = 0; i < PIXEL_BYTES*BYTE_SIZE; i++){
// 	// 	for (j = 0; j < 8; j++){
// 	// 		gpioWrite(PTC9, HIGH);
// 	// 	}

// 	// }
// }

// /*******************************************************************************
//  *******************************************************************************
//                         LOCAL FUNCTION DEFINITIONS
//  *******************************************************************************
//  ******************************************************************************/

// void toggle_DC(){
//     if (bit){
//         PWM_SetON(FTM_ID, USEC2TICKS(T0H, LED_MATRIX_PRESCALER));
//     } else {
//         PWM_SetON(FTM_ID, USEC2TICKS(T1H, LED_MATRIX_PRESCALER));
//     }
//     bit = !bit;
// }

// /*******************************************************************************
//  ******************************************************************************/

