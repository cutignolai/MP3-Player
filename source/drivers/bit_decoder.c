// /***************************************************************************//**
//   @file     demodulador.c
//   @brief    demodulator driver
//   @author   Olivia De Vincenti, Pedro DL
//  ******************************************************************************/

// /*******************************************************************************
//  * INCLUDE HEADER FILES
//  ******************************************************************************/
// #include "bit_decoder.h"
// #include "MK64F12.h"
// #include "gpio.h"
// #include "FTM.h"
// #include "uart.h"
// #include "DMA.h"
// #include "CMP.h"
// /*******************************************************************************
//  * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
//  ******************************************************************************/
// #define OUT_PIN	PORTNUM2PIN(PD, 1)
// #define MEDIAN(x1, x2)		(((x2) - (x1))/2 + (x1))	// Asume x2 > x1
// #define FSK1	1200
// #define T1		833/2
// #define FSK2	2200
// #define T2		400/2
// #define IC_PRESCALER	FTM_PSC_x16
// #define UART_SEL 0
// #define IC_FTM 2
// #define IC_CH 0
// #define CURRENT_CARRY FTM
// #define MAX_COUNT	0xFFFF

// /*******************************************************************************
//  * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
//  ******************************************************************************/
// typedef enum {FTM, DMA} carry_t;
// /*******************************************************************************
//  * VARIABLES WITH GLOBAL SCOPE
//  ******************************************************************************/

// /*******************************************************************************
//  * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
//  ******************************************************************************/

// /*******************************************************************************
//  * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
//  ******************************************************************************/

// /*******************************************************************************
//  * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
//  ******************************************************************************/
// static char word;
// static uint8_t new_word;
// static uint8_t w_counter;

// //DMA
// FTM_Type* FTMs [] = FTM_BASE_PTRS;
// static uint16_t cnv_value[100];



// /*******************************************************************************
//  *******************************************************************************
//                         GLOBAL FUNCTION DEFINITIONS
//  *******************************************************************************
//  ******************************************************************************/
// void b_decoder_init(void){

//     CMP_full_init(CMP_0);

//     gpioMode(OUT_PIN, OUTPUT);

//     //FTM INIT
//     if (CURRENT_CARRY == FTM){
        
//         ftm_channel_config_t ch1 = { .channel = IC_CH, .mode = INPUT_CAPTURE, .options = BOTH_EDGES, .dma = false, .pin = PORTNUM2PIN(PB, 18), .pin_mux = 3 };
//         ftm_config_t ftm0 = { .prescaler = IC_PRESCALER, .count_mode = UP_COUNT, .overflow = true, .input_clock = BUS_CLK, .trigger = NO_TRIGGER, .trigger_options = 0, .channel_n = 1, .channel_arr[0] = ch1 };
//         FTM_Init(IC_FTM, ftm0);
//         IC_Init(IC_FTM, 50);
//     }

//     //DMA START
//     else if (CURRENT_CARRY == DMA){
//         ftm_channel_config_t ch1 = { .channel = IC_CH, .mode = INPUT_CAPTURE, .options = RISING_EDGE, .dma = true, .pin = PORTNUM2PIN(PC, 2), .pin_mux = 4 };
//         ftm_config_t ftm0 = { .prescaler = FTM_PSC_x1, .count_mode = UP_COUNT, .overflow = false, .input_clock = BUS_CLK, .trigger = NO_TRIGGER, .trigger_options = 0, .channel_n = 1, .channel_arr[0] = ch1 };
//         FTM_Init(IC_FTM, ftm0);
//         IC_Init(IC_FTM, 50);
        
//         DMA_config_t config_dma_output = {
//         .source_buffer = &(FTM0->CONTROLS[1].CnV),
//         // .destination_buffer = &(seno_clase[0]),
//         .destination_buffer = (uint32_t)cnv_value,
//         .source_offset = 0x02, //puede ser 4
//         .destination_offset = 0,
//         .transfer_bytes = 4,
//         .major_cycles = (100),
//         .wrap_around = 100*sizeof(uint32_t)};
//         DMA_Config(0, config_dma_output);




//     }
    
//     //UART INIT
//     uart_cfg_t u_config;
//     u_config.baud_rate = 9600;
//     u_config. non_blocking = false;
//     u_config.want_parity = true;
//     u_config.data_9bits = false;
//     u_config.parity_type = PARITY_ODD;
//     u_config.double_stop_bit = false;
//     u_config.use_fifo = true;
//     u_config.bit_rate = 0;
//     uartInit (UART_SEL, u_config);

//     //OTHER STUFF
//     w_counter = 0;
//     FTM_StartTimer(IC_FTM);

// }

// void decode_bits(void){
//     static bool segu = false;
//     if (IC_IsNewData(IC_FTM)){
//     	static uint16_t IC_data;
//         if (CURRENT_CARRY == FTM){
//             IC_data = IC_GetData(IC_FTM);
//         }
//         else if (CURRENT_CARRY == DMA){

//             IC_SetNewData(IC_FTM);
//         }
        
//         if (IC_data > USEC2TICKS(MEDIAN(T2, T1), IC_PRESCALER)){
//             gpioWrite(OUT_PIN, 1);
//             new_word = new_word<<1;
//             new_word |= 0b1;
//             w_counter++;
//             if(w_counter == 9){
//                 word = (char) new_word;
//                 w_counter = 0;
//                 new_word = 0;
//                 uartWriteMsg(UART_SEL, &word, 1);
//             }
//         } 
//         else {
//             if (!segu){
//                 gpioWrite(OUT_PIN, 0);
//                 segu = true;
//                 new_word = new_word<<1;
//                 new_word |= 0b0;
//                 w_counter++;
//                 if(w_counter == 9){
//                     word = (char) new_word;
//                     w_counter = 0;
//                     new_word = 0;
//                     uartWriteMsg(UART_SEL, &word, 1);
//                 }
//             } 
//             else {
//                 segu = false;
//             }
//         }
//     }
//  }
 



// /*******************************************************************************
//  *******************************************************************************
//                         LOCAL FUNCTION DEFINITIONS
//  *******************************************************************************
//  ******************************************************************************/

// /******************************************************************************/
