/***************************************************************************//**
  @file     led_matrix.h
  @brief    LED Matrix Driver
  @author   Olivia De Vincenti
 ******************************************************************************/

/*******************************************************************************
 *                      INCLUDE HEADER FILES                                    *
 ******************************************************************************/
#include "gpio.h"
#include "board.h"
#include "timer.h"
#include "led_matrix.h"

/*******************************************************************************
 *          CONSTANT AND MACRO DEFINITIONS USING #DEFINE                        *
 ******************************************************************************/

#define PIXEL_BYTES		3

/***** PIXEL TIMING *****/
#define	Ts			1.25 //us
#define T0H			0.4 //us
#define T0L			0.85 //us
#define T1H			0.8 //us
#define T1L			0.45 //us
#define RES			50 //us

/**** OVERALL TIMING ****/
#define REFRESH_PERIOD	16000 //us

/******* PINS *******/
#define DIN     PTC9
#define DOUT    PTC10

#define N_PINS  8

#ifdef LED_MATRIX_DEV_MODE
	#define LED_MATRIX_TEST_PIN     PTC4
#endif


/*******************************************************************************
 *               ENUMERATIONS AND STRUCTURES AND TYPEDEFS                       *
 ******************************************************************************/

/*******************************************************************************
 *                  VARIABLE PROTOTYPES WITH GLOBAL SCOPE                       *
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
// void refresh_display(void);
// void set_segment(uint8_t seg, uint8_t set);
// void set_display(digit_t arr[]);
// void set_digit(digit_t number, uint8_t position);
// void set_buffer(digit_t *dig_arr, uint8_t arr_len);
// void clear_buffer();
// void blink_digits();
// void scroll_buffer();
// void scroll_message();
// void display_blink(digit_t* arr);
// void clear_display();
// void auto_set_buffer_index();

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
// digit_t char_arr[] = {NUM_0, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, DASH, LOW_DASH, POINT, CLEAR, 
// LET_A, LET_a, LET_b, LET_C, LET_d, LET_E, LET_e, LET_F, LET_G, LET_g, LET_H, LET_h, LET_I, LET_J, LET_j, LET_l, LET_L, LET_n, 
// LET_O, LET_P, LET_Q, LET_r, LET_S, LET_t, LET_U, LET_X, LET_Y };

// uint8_t seg_arr[] = {SEGA, SEGB, SEGC, SEGD, SEGE, SEGF, SEGG, SEGDP};

// display_states_t display_state = DISPLAY_CLEAR;
// uint8_t display_idx = 0;        // Índice que indica qué dígito del display configurar

// digit_t buffer[BUFFER_MAX_LEN + 1] = {IDX_CLEAR, IDX_CLEAR, IDX_CLEAR, IDX_CLEAR, IDX_CLEAR, IDX_CLEAR, IDX_CLEAR, IDX_CLEAR, IDX_CLEAR, IDX_CLEAR, IDX_CLEAR, IDX_CLEAR, IDX_CLEAR, IDX_CLEAR, IDX_CLEAR, IDX_CLEAR, IDX_CLEAR};
// uint8_t buffer_len;         // Cantidad de caracteres seteados del buffer
// uint8_t buffer_idx = 0;         // Caracter a partir de la cual mostrar

// uint8_t brightness = BRIGHTNESS_LOW;
// uint8_t brightness_count = 0;
// uint8_t intensity = MAX_INTENSITY >> BRIGHTNESS_LOW;

// uint8_t scroll_idx;         // Índice que indica el dígito del buffer que se va a mostrar en la posición 1 en ese instante de scroll

// bool show_last = false;

// bool blink = false;
// bool blinking_digits[DISPLAY_LEN] = { false, false, false, false};

// // TIMERS
// tim_id_t refresh_timer;
// tim_id_t blink_timer;
// tim_id_t scroll_timer;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
// INIT
// void initLedMatrix(){

// 	gpioMode(DIN, OUTPUT);
//     gpioMode(DOUT, INPUT);

// // #ifdef LED_MATRIX_DEV_MODE
// // 	gpioMode(LED_MATRIX_TEST_PIN, OUTPUT);
// // #endif

//     // CREATE TIMERS
//     timerInit();
//     refresh_timer = timerGetId();
//     timerCreate(refresh_timer, TIMER_MS2TICKS(MAX_REFRESH_T), TIM_MODE_PERIODIC, refresh_display);
//     blink_timer = timerGetId();
//     timerCreate(blink_timer, TIMER_MS2TICKS(BLINK_T), TIM_MODE_PERIODIC, blink_digits);
//     scroll_timer = timerGetId();
//     timerCreate(scroll_timer, TIMER_MS2TICKS(SCROLL_T), TIM_MODE_PERIODIC, scroll_buffer);

//     setBrightness(BRIGHTNESS_MEDIUM);

//     // INIT 7 SEGMENTS
//     buffer_len = BUFFER_MAX_LEN;
//     clear_buffer();
//     buffer[BUFFER_MAX_LEN] = IDX_CLEAR;
//     clear_display();

//     // START DISPLAY
//     timerActivate(refresh_timer);
// }                     

// void setClearMode(){
//     if (display_state != DISPLAY_CLEAR){
//         timerReset(blink_timer);
//         timerReset(scroll_timer);
//     }
//     display_state = DISPLAY_CLEAR;
// }

// void setScrollMode(){
//     if (display_state != DISPLAY_SCROLL){
//         timerReset(blink_timer);
//         timerRestart(scroll_timer);
//         scroll_idx = 0;
//     }
//     display_state = DISPLAY_SCROLL;
// }

// void setStaticMode(){
//     if (display_state != DISPLAY_STATIC){
//         timerReset(blink_timer);
//         timerReset(scroll_timer);
//     }
//     display_state = DISPLAY_STATIC;
// }

// void setBlinkMode(){
//     if (display_state != DISPLAY_BLINK){
//         timerReset(blink_timer);
//         timerReset(scroll_timer);
//         blink = false;
//         timerRestart(blink_timer);
//     }
//     display_state = DISPLAY_BLINK;
// }

// void loadBuffer(digit_t *msg, uint8_t msg_len){
//     if (msg_len > BUFFER_MAX_LEN) { msg_len = BUFFER_MAX_LEN; }
//     set_buffer(msg, msg_len);
//     if (show_last) { auto_set_buffer_index(); }
// }

// void showLastDigits(bool b){
//     show_last = b;
//     if (show_last) { auto_set_buffer_index(); }
// }

// void setBufferIndex(uint8_t idx){
//     if (idx < BUFFER_MAX_LEN - DISPLAY_LEN){
//         buffer_idx = idx;
//     }
// }

// void setBlinkingDigits(bool* arr){
//     uint8_t i;
//     for (i = 0; i < DISPLAY_LEN; i++){
//         blinking_digits[i] = *(arr + i);
//     }
// }

// void setBrightness(brightness_states_t bright){
//     if ( BRIGHTNESS_LOW <= bright && bright <= BRIGHTNESS_HIGH ){
//         brightness = bright;
//         intensity = MAX_INTENSITY >> brightness;
//     }
// }

// void upBrightness(){
//     setBrightness(brightness + 1);
// }

// void downBrightness(){
//     setBrightness(brightness - 1);
// }

// uint8_t getBrightnessState(){
//     return brightness;
// }

// /*******************************************************************************
//  *******************************************************************************
//                         LOCAL FUNCTION DEFINITIONS
//  *******************************************************************************
//  ******************************************************************************/

// void refresh_display()
// {

// #ifdef DISPLAY_DEV_MODE
// 	gpioWrite(DISPLAY_TEST_PIN, 1);
// #endif

//     if( ! ( brightness_count % intensity ) ){

//         switch (display_state)
//         {
//             case DISPLAY_SCROLL:
//                 if (buffer_len <= DISPLAY_LEN){
//                     set_digit(buffer[display_idx], display_idx);
//                 } else {
//                     scroll_message();
//                 }
//                 break;

//             case DISPLAY_BLINK:
//                 if (blink && blinking_digits[display_idx]){
//                     set_digit(IDX_CLEAR, display_idx);
//                 } else {
//                 set_digit(buffer[buffer_idx + display_idx], display_idx);
//                 }
//                 break;

//             case DISPLAY_STATIC:
//                 set_digit(buffer[buffer_idx + display_idx], display_idx);
//                 break;

//             case DISPLAY_CLEAR:
//             default:
//                 clear_display();
//                 break;
//         }
//     } else {
//     	set_digit(IDX_CLEAR, DISPLAY_LEN - 1);
//     }
    
// 	display_idx++;

//     if ( display_idx == DISPLAY_LEN){ 
//         display_idx = 0; 
//         brightness_count++;
//         if ( brightness_count >= MAX_INTENSITY ){
//             brightness_count = 0;
//         }
//     }

// #ifdef DISPLAY_DEV_MODE
// 	gpioWrite(DISPLAY_TEST_PIN, 0);
// #endif

// }

// void scroll_message() {

//     set_digit(buffer[(scroll_idx + display_idx) % (buffer_len + 1)], display_idx);
//     if (scroll_idx == buffer_len + 1){
//     	scroll_idx = 0;
//     }
//     //set_digit(buffer[CLEAR], display_idx);
//     if (display_idx == DISPLAY_LEN){
//     	scroll_idx++;
//     }
// }

// void clear_display(){
//     clear_buffer();
//     buffer_idx = 0;
// }

// void set_display(digit_t* arr){
//     uint8_t i;
//     for (i = 0; i < DISPLAY_LEN; i++){
//         set_digit(*(arr + i), i);
//     }
// }

// void set_digit(digit_t number, uint8_t position){

//     digit_t character = char_arr[number];
    
//     // Select digit
//     gpioWrite(SEL0, GET_SEL0(position));
//     gpioWrite(SEL1, GET_SEL1(position));

//     // Segments ON
//     uint8_t idx;
//     for (idx = 0; idx < SEGMENTS; idx++){
//         uint8_t set = character & 1;
//         set_segment(seg_arr[idx], set);
//         character = character >> 1;
//     }
// }

// void set_segment(uint8_t seg, uint8_t set){
//     gpioWrite(seg, set);
// }

// void set_buffer(digit_t *dig_arr, uint8_t arr_len){
//     uint8_t i;
//     clear_buffer();
//     clear_display();
//     for (i = 0; i < arr_len; i++){
//         buffer[i] = *(dig_arr + i);
//     }
//     buffer_len = arr_len;
// }

// void auto_set_buffer_index(){
//     if (buffer_len > DISPLAY_LEN){
//         buffer_idx = buffer_len - DISPLAY_LEN;
//     } else {
//         buffer_idx = 0;
//     }
// }

// void clear_buffer(void)
// {
//     uint8_t i;
//     for (i = 0; i < buffer_len; i++){
//     	buffer[i] = IDX_CLEAR;
//     }
//     buffer_len = 0;
// }

// void display_blink(digit_t* arr){
//     uint8_t i;
//     for (i = 0; i < DISPLAY_LEN; i++){
//         if (blink && blinking_digits[i]){
//             set_digit(IDX_CLEAR, i);
//         } else {
//             set_digit(*(arr + i), i);
//         }
//     }
// }

// // callback scroll
// void scroll_buffer(){
//     scroll_idx++;
// }

// // callback blink
// void blink_digits(){
//     blink = !blink;
// }

// /*******************************************************************************
//  ******************************************************************************/
