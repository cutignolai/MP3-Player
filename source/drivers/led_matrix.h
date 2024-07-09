/***************************************************************************//**
  @file     display.h
  @brief    Display Driver
  @author   Olivia De Vincenti
 ******************************************************************************/
#ifndef _LED_MATRIX_
#define _LED_MATRIX_

/*******************************************************************************
 *                      INCLUDE HEADER FILES                                    *
 ******************************************************************************/

/*******************************************************************************
 *          CONSTANT AND MACRO DEFINITIONS USING #DEFINE                        *
 ******************************************************************************/

#define MATRIX_COLS  8
#define MATRIX_ROWS  8


/*******************************************************************************
 *               ENUMERATIONS AND STRUCTURES AND TYPEDEFS                       *
 ******************************************************************************/
typedef struct {
    uint8_t green;
    uint8_t red;
    uint8_t blue;
} matrix_pixel_t;

#define RED_PIXEL {.green = 0, .red = 128, .blue = 0}
#define YELLOW_PIXEL {.green = 128, .red = 128, .blue = 0}
#define PURPLE_PIXEL {.green = 0, .red = 255, .blue = 255}
#define ORANGE_PIXEL {.green = 64, .red = 200, .blue = 0}
#define GREEN_PIXEL {.green = 128, .red = 0, .blue = 0}
#define BLUE_PIXEL {.green = 0, .red = 0, .blue = 128}
#define OFF_PIXEL {.green = 0, .red = 0, .blue = 0}

/*******************************************************************************
 *                  VARIABLE PROTOTYPES WITH GLOBAL SCOPE                       *
 ******************************************************************************/

/*******************************************************************************
 *                   FUNCTION PROTOTYPES WITH GLOBAL SCOPE                      *
 ******************************************************************************/
void initLedMatrix();
void showMatrix();
void updateMatrix(matrix_pixel_t* matrix_pixels);
void setBrightness(int br);

/*******************************************************************************
 ******************************************************************************/

#endif // _LED_MATRIX_H_
