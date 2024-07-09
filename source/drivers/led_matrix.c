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
#include "DMA.h"
#include "FTM.h"
// #include "timer.h"
#include "led_matrix.h"

/*******************************************************************************
 *          CONSTANT AND MACRO DEFINITIONS USING #DEFINE                        *
 ******************************************************************************/

#define MAX_BRIGHT		100
#define MIN_BRIGHT		1

#define PIXEL_BYTES		3
#define BYTE_SIZE       8

/***** PIXEL TIMING *****/
// Ts			1.25 us
// T0H			0.4 us
// T0L			0.85 us
// T1H			0.8 us
// T1L			0.45 us
// RES			50 us

#define HIGH_BIT_DC     41
#define LOW_BIT_DC      21

#define RESET_PERIOD    50 //us

/******* PINS *******/
#ifdef LED_MATRIX_DEV_MODE
	#define LED_MATRIX_TEST_PIN     PTC4
#endif

#define FTM_ID  0
#define PWM_CH  1
#define FTM_CH_PIN	PORTNUM2PIN(PC, 2)
#define LED_MATRIX_PRESCALER FTM_PSC_x1

#define DMA_ID	DMA_0

/*******************************************************************************
 *               ENUMERATIONS AND STRUCTURES AND TYPEDEFS                       *
 ******************************************************************************/

/*******************************************************************************
 *                  VARIABLE PROTOTYPES WITH GLOBAL SCOPE                       *
 ******************************************************************************/
uint16_t matrix_buffer[MATRIX_ROWS*MATRIX_COLS*PIXEL_BYTES*BYTE_SIZE] __attribute__((aligned(16)));

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
void loadMatrix(matrix_pixel_t* matrix_pixels);
void byte2DC(uint8_t byte, uint16_t* pixel);

void DMA_callback();
void DMA_Error_callback();

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static bool update_matrix = false;
static matrix_pixel_t* matrix_src = 0;
static int brightness = MAX_BRIGHT;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void initLedMatrix(){

// #ifdef LED_MATRIX_DEV_MODE
// 	gpioMode(LED_MATRIX_TEST_PIN, OUTPUT);
// #endif

    ftm_channel_config_t ch1 = { .channel = PWM_CH, .mode = EDGE_PWM, .options = HIGH_TRUE, .dma = true, .pin = FTM_CH_PIN, .pin_mux = 4 };
	ftm_config_t ftm0 = { .prescaler = FTM_PSC_x1, .count_mode = UP_COUNT, .overflow = true, .input_clock = BUS_CLK, .trigger = NO_TRIGGER, .trigger_options = 0, .channel_n = PWM_CH, .channel_arr[0] = ch1 };
	FTM_Init(FTM_ID, ftm0);
	PWM_Init(FTM_ID, PWM_CH, 62, matrix_buffer[0]);

	DMA_config_t dmaconfig = {
			.channel = DMA_ID,
			.request_source = DMA_FTM0CH1,
			.trigger = false,
			.source_buffer = (uint16_t*) &matrix_buffer[0],
			.destination_buffer = (uint16_t*) (&FTM_PTRS[FTM_ID]->CONTROLS[PWM_CH].CnV),
			.source_offset = sizeof(matrix_buffer[0]),		// Source offset de 2 bytes xq CnV es de 16 bits
			.destination_offset = 0x00,						// Siempre va al mismo lugar (a CnV)
			.transfer_size = sizeof(matrix_buffer[0]),		// Transferencias de 16 bits (2 bytes) xq CnV es de 16 bits
			.source_module = 0,
			.destination_module = 0,
			.minor_loop_count = sizeof(matrix_buffer[0]),	// 2 bytes por minor loop xq CnV es de 16 bits
			.mayor_loop_count = sizeof(matrix_buffer)/sizeof(matrix_buffer[0])+1,		// Cada mayor loop equivale a un recorrido del arreglo
			.mayor_loop_offset_s = -(sizeof(matrix_buffer)+2),		// Reinicia el arreglo luego de cada mayor loop
			.mayor_loop_offset_d = 0,							// Mismo destino luego de cada mayor loop
			.scatter_gather = false,
	} ;

	DMA_Config(dmaconfig);
	DMA_SetCallback(DMA_ID, &DMA_callback);
	DMA_SetErrorCallback(DMA_ID, &DMA_Error_callback);
	//FTM_StartTimer(FTM_ID);
    //DMA_DisableChannel(PWM_CH);

}                     

void showMatrix(){
	DMA_EnableChannel(PWM_CH);
    FTM_StartTimer(FTM_ID);
}

void updateMatrix(matrix_pixel_t* matrix_pixels){
	if (DMA_IsTransfer(PWM_CH)){
		update_matrix = true;
		matrix_src = matrix_pixels;
	} else {
		loadMatrix(matrix_pixels);
	}
}

void setBrightness(int br){
	if (br != brightness && MIN_BRIGHT <= br && br <= MAX_BRIGHT){
		brightness = br;
	}
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void loadMatrix(matrix_pixel_t* matrix_pixels){
	uint8_t i;
    for (i = 0; i < MATRIX_ROWS*MATRIX_COLS; i++){
        byte2DC((int) matrix_pixels[i].green*brightness/MAX_BRIGHT, &matrix_buffer[i*PIXEL_BYTES*BYTE_SIZE]);
        byte2DC((int) matrix_pixels[i].red*brightness/MAX_BRIGHT, &matrix_buffer[i*PIXEL_BYTES*BYTE_SIZE + BYTE_SIZE]);
        byte2DC((int) matrix_pixels[i].blue*brightness/MAX_BRIGHT, &matrix_buffer[i*PIXEL_BYTES*BYTE_SIZE + BYTE_SIZE*2]);
    }
}

void byte2DC(uint8_t byte, uint16_t* pixel){
    int8_t i;
    uint8_t mask = 1;
    for (i = 7; i >= 0; i--){
        if (byte & mask){
            pixel[i] = HIGH_BIT_DC;
        } else {
            pixel[i] = LOW_BIT_DC;
        }
        mask = mask << 1;
    }
}

/* The blue LED is toggled when a TCD is completed. */
void DMA_callback(){
	/* Toggle the blue LED. */
	FTM_StopTimer(FTM_ID);
	//DMA_DisableChannel(PWM_CH);
	if (update_matrix){
		loadMatrix(matrix_src);
	}
	update_matrix = false;
}

/* The red LED is toggled when an error occurs. */
void DMA_Error_callback(){
	uint8_t i = 0;
}


/*******************************************************************************
 ******************************************************************************/
