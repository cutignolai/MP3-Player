/*
 * main.c
 *
 *  Created on: May 1, 2015
 *      Author: Juan Pablo VEGA - Laboratorio de Microprocesadores
 */

#include "hardware.h"
#include "drivers/gpio.h"
#include "drivers/DAC.h"
#include "drivers/DMA.h"

#define PIN_BLUE_LED 		PORTNUM2PIN(PB, 21)    	//PTB21
#define PIN_RED_LED 		PORTNUM2PIN(PB, 22)    	//PTB22
#define PIN_GREEN_LED 		PORTNUM2PIN(PE, 26)  	//PTE26

#define DAC_ID  DAC_0
#define DMA_ID	DMA_1

#define SAMPLE_BUFFER_SIZE 2304
int16_t sample_buffer[SAMPLE_BUFFER_SIZE];

static void delayLoop(uint32_t veces);
void DMA_callback();
void DMA_Error_callback();

void App_Init (void)
{
	initDAC(DAC_ID);
    DACEnableDMA(DAC_ID);

	DMA_config_t dmaconfig = {
			.channel = DMA_ID,
			.request_source = DMA_DAC0,
			.trigger = false,
			.source_buffer = (uint32_t) &sample_buffer[0],
			.destination_buffer = (uint32_t) &(DAC0->DAT),
			.source_offset = sizeof(sample_buffer[0]),		// Source offset de 2 bytes xq el sample_buffer es de 16 bits
			.destination_offset = 0x00,						// Siempre va al mismo lugar (a DAT del DAC0)
			.transfer_size = sizeof(sample_buffer[0]),		// Transferencias de 16 bits (2 bytes) xq sample_buffer es de 16 bits
			.source_module = 0,
			.destination_module = 0,
			.minor_loop_count = sizeof(sample_buffer[0]),	// 2 bytes por minor loop
			.mayor_loop_count = sizeof(sample_buffer)/(2*sizeof(sample_buffer[0])),		// Cada mayor loop equivale a medio recorrido del arreglo
			.mayor_loop_offset_s = 0,		                // NO reinicia el arreglo luego de cada mayor loop
			.mayor_loop_offset_d = 0,						// Mismo destino luego de cada mayor loop
			.scatter_gather = false,
	} ;

	DMA_Config(dmaconfig);
	DMA_SetCallback(DMA_ID, &DMA_callback);
	DMA_SetErrorCallback(DMA_ID, &DMA_Error_callback);

	gpioMode(PIN_BLUE_LED, OUTPUT);
	gpioMode(PIN_RED_LED, OUTPUT);
	gpioMode(PIN_GREEN_LED, OUTPUT);

	/* Turn all the LEDs off. (Active low)*/
	gpioWrite(PIN_BLUE_LED, 1);
	gpioWrite(PIN_RED_LED, 1);
	gpioWrite(PIN_GREEN_LED, 1);

	FTM_StartTimer(FTM_ID);

}


void App_Run(void)
{

}

static void delayLoop(uint32_t veces)
{
    while (veces--);
}

/* The blue LED is toggled when a TCD is completed. */
void DMA_callback(){
	/* Toggle the blue LED. */
	FTM_StopTimer(FTM_ID);
	delayLoop(800UL);
	gpioToggle(PIN_BLUE_LED);
	FTM_StartTimer(FTM_ID);
}

/* The red LED is toggled when an error occurs. */
void DMA_Error_callback(){
	/* Turn the red LED on. */
	gpioWrite(PIN_RED_LED, 0);
}
