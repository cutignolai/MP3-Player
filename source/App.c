/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Cuti, Micho, Oli
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdio.h>
#include <string.h>

// A revisar
#include <os.h>

// Drivers
#include <drivers/button.h>
#include <drivers/board.h>
#include <drivers/gpio.h>
#include <drivers/timer.h>
#include <drivers/leds.h>
#include <drivers/encoder.h>
#include <drivers/button.h>
#include <drivers/user.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

//----------- ESTADOS DEL MENU -----------
typedef enum{
    ESTADO_IDLE,
    ESTADO_RIGHT,
    ESTADO_LEFT,
    ESTADO_CLICK
} estadosDelMenu_t;
//----------------------------------------

//----------- EVENTOS DEL MENU -----------
typedef enum{
    EVENTO_NONE,
    EVENTO_IZQ,
    EVENTO_DER,
    EVENTO_CLICK,
    EVENTO_CLICK_2,
    EVENTO_CLICK_3,
    EVENTO_CLICK_LONG,
    EVENTO_TARJETA,
    EVENTO_MSG
} eventosDelMenu_t;
//----------------------------------------

//----------- COLORES DEL LED ------------
typedef enum{
    LED1,
    LED2,
    LED3
} colored_led_t;
//-----------------------------------------


#define MAX_UNIT_ID         8
#define MIN_UNIT_PASS		4
#define MAX_UNIT_PASS       5

#define READY               true
#define NOT_READY           !READY

#define USUARIO_VALIDO      true
#define USUARIO_INVALIDO    !USUARIO_VALIDO

#define ENTER_CLICK         1
#define BRILLO_CLICK        2
#define SUPR_CLICK          3
 
#define SI                  true
#define NO                  false

#define ACTIVADO            true
#define DESACTIVADO         false

#define OPEN_TIME           5000
#define SEC                 1000
#define WRONG_TIME_1        5
#define WRONG_TIME_2        30

#define SEM_AMMOUNT			4

/*******************************************************************************
 * ENUMS AND STRUCTURES
 ******************************************************************************/




/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static estadosDelMenu_t idle(eventosDelMenu_t evento);
static estadosDelMenu_t modificar_right(eventosDelMenu_t evento);
static estadosDelMenu_t modificar_left(eventosDelMenu_t evento);
static estadosDelMenu_t modificar_click(eventosDelMenu_t evento);


/*******************************************************************************
 * VARIABLES
 ******************************************************************************/



// ESTADOS
static estadosDelMenu_t estado = ESTADO_IDLE;
static estadosDelMenu_t ultimo_estado = ESTADO_IDLE;




// TIMERS
static tim_id_t sec_timer;
static uint8_t sec_count;
static uint8_t wrong_count;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (OS_Q* queue)
{
    initEncoder();
	initButton();
    initLeds();
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	eventosDelMenu_t evento = EVENTO_NONE;

	if(encoderGetStatus())
	{
		evento = encoderGetEvent();
		encoderSetStatus(DESACTIVADO);

	}
	else if(buttonGetStatus())
	{
		evento = buttonGetEvent();
		buttonSetStatus(DESACTIVADO);
	}

	// Si hubo un evento, veo en que estado de mi FSM estoy y le envio el evento
	if(evento != EVENTO_NONE)
	{
		switch(estado){
			case ESTADO_IDLE:
				estado = idle(evento);
				break;
			case ESTADO_RIGHT:
				estado = modificar_right(evento);
				break;
			case ESTADO_LEFT:
				estado = modificar_left(evento);
				break;
			case ESTADO_CLICK:
				estado = modificar_click(evento);
				break;
			default: break;
		}
	}


}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static estadosDelMenu_t idle(eventosDelMenu_t evento)
{
	estadosDelMenu_t proximo_estado = ESTADO_IDLE;

    switch(evento)
    {
        case EVENTO_DER:
            proximo_estado = ESTADO_RIGHT;
            break;

        case EVENTO_IZQ:
            proximo_estado = ESTADO_LEFT;
            break;


        case EVENTO_CLICK:
            proximo_estado = ESTADO_CLICK;
            break;
            
        default:
            break;

    }

	return proximo_estado;
}

static estadosDelMenu_t modificar_right(eventosDelMenu_t evento)
{
	estadosDelMenu_t proximo_estado = ESTADO_IDLE;
    clear_leds();
    set_led(0);

	return proximo_estado;
}

static estadosDelMenu_t modificar_left(eventosDelMenu_t evento)
{
	estadosDelMenu_t proximo_estado = ESTADO_IDLE;
    clear_leds();
    set_led(1);

	return proximo_estado;
}

static estadosDelMenu_t modificar_click(eventosDelMenu_t evento)
{
	estadosDelMenu_t proximo_estado = ESTADO_IDLE;
    clear_leds();
    set_led(2);

	return proximo_estado;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/





/*******************************************************************************
 ******************************************************************************/

