/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdio.h>
// #include <os.h>
// #include <string.h>
#include <drivers/board.h>
#include <drivers/gpio.h>
#include <drivers/led_matrix.h>
// #include <drivers/leds.h>
// #include <drivers/encoder.h>
// #include <drivers/card_reader.h>
// #include <drivers/button.h>
// #include <drivers/user.h>
// #include <drivers/message.h>
#include "MK64F12.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMS AND STRUCTURES
 ******************************************************************************/




/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static void delayLoop(uint32_t veces);

/*******************************************************************************
 * VARIABLES
 ******************************************************************************/


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	matrix_pixel_t matrix_pixels[64] = {
        OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL,
		OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL,
		OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL,
		OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL,
		OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL,
		OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL,
		OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL,
		OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL, OFF_PIXEL
    };
    updateMatrix(&matrix_pixels[0]);
    initLedMatrix();
    showMatrix();
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{

    delayLoop(8000000UL);

	matrix_pixel_t more_pixels[64] = {
		GREEN_PIXEL, RED_PIXEL, BLUE_PIXEL, YELLOW_PIXEL, ORANGE_PIXEL, PURPLE_PIXEL, RED_PIXEL, BLUE_PIXEL,
		GREEN_PIXEL, RED_PIXEL, BLUE_PIXEL, YELLOW_PIXEL, ORANGE_PIXEL, PURPLE_PIXEL, RED_PIXEL, BLUE_PIXEL,
		GREEN_PIXEL, RED_PIXEL, BLUE_PIXEL, YELLOW_PIXEL, ORANGE_PIXEL, PURPLE_PIXEL, RED_PIXEL, BLUE_PIXEL,
		GREEN_PIXEL, RED_PIXEL, BLUE_PIXEL, YELLOW_PIXEL, ORANGE_PIXEL, PURPLE_PIXEL, RED_PIXEL, BLUE_PIXEL,
		GREEN_PIXEL, RED_PIXEL, BLUE_PIXEL, YELLOW_PIXEL, ORANGE_PIXEL, PURPLE_PIXEL, RED_PIXEL, BLUE_PIXEL,
		GREEN_PIXEL, RED_PIXEL, BLUE_PIXEL, YELLOW_PIXEL, ORANGE_PIXEL, PURPLE_PIXEL, RED_PIXEL, BLUE_PIXEL,
		GREEN_PIXEL, RED_PIXEL, BLUE_PIXEL, YELLOW_PIXEL, ORANGE_PIXEL, PURPLE_PIXEL, RED_PIXEL, BLUE_PIXEL,
		GREEN_PIXEL, RED_PIXEL, BLUE_PIXEL, YELLOW_PIXEL, ORANGE_PIXEL, PURPLE_PIXEL, RED_PIXEL, BLUE_PIXEL
	};
	setBrightness(1);
    updateMatrix(&more_pixels[0]);
    showMatrix();

    delayLoop(8000000UL);

    matrix_pixel_t matrix_pixels[64] = {
		RED_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, PURPLE_PIXEL,
		RED_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, PURPLE_PIXEL,
		RED_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, PURPLE_PIXEL,
		RED_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, PURPLE_PIXEL,
		RED_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, PURPLE_PIXEL,
		RED_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, PURPLE_PIXEL,
		RED_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, PURPLE_PIXEL,
		RED_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, GREEN_PIXEL, PURPLE_PIXEL
	};
    setBrightness(1);
    updateMatrix(&matrix_pixels[0]);
    showMatrix();

}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void delayLoop(uint32_t veces)
{
    while (veces--);
}

/*******************************************************************************
 ******************************************************************************/

