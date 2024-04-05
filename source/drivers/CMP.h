/***************************************************************************//**
  @file     CMP.c
  @brief    CMP Driver for K64F
  @author   Pedro DL
 ******************************************************************************/
#ifndef _CMP_H_
#define _CMP_H_
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef enum{
    IN0,
    IN1,
    IN2,
    IN3,
    IN4,
    IN5,
    IN6,
    IN7
}CMP_input;

typedef enum{
    FTM1_CH0,
    FTM2_CH0
}CMP_output;

typedef enum{
    h_lvl_0,
    h_lvl_1,
    h_lvl_2,
    h_lvl_3
}hysteresis_lvl;

typedef enum{
    Vin1,
    Vin2
}CMP_Vref;

typedef enum{
    CMP_0,
    CMP_1,
    CMP_2
}CMP_t;

typedef struct{
    bool DAC_en;
    CMP_Vref Vref;
    uint8_t output_vol;         //(0-63) DACO = (Vin /64) * (output_vol + 1)
}DAC_cfg_t;

typedef struct {
    hysteresis_lvl h_lvl;
    bool high_speed;
    bool inverter;
    bool output_pin_en;
    DAC_cfg_t DAC;
}CMP_cfg_t;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Gives a basic CMP config if none is set
 * @return a struct wich is limited yet functional
*/
CMP_cfg_t CMP_get_cfg_default (void);
/**
 * @brief Initialize CMP driver. Note that both output and inputs must be set appart
 * @param config CMP's configuration 
 * @param n CMP's number
*/
void CMP_init(CMP_cfg_t config, CMP_t n);
/**
 * @brief manages CMPs interrupts
 * @param en enable/disable the CMP interrupts
 * @param n CMP's number
*/
void CMP_manage_interrupts(bool en, CMP_t n);
/**
 * @brief sets CMP sample filter
 * @param sample_count indicates how many samples must agree prior to the CMP output filter accepting a new state. Range 1-7, 0 to disable
 * @param period Specifies the sampling period, in bus clock cycles, of the comparator output filter. Range 0-255
 * @param n CMP's number
*/
void CMP_set_filter(CMP_t n, uint8_t sample_count, uint8_t period);
/**
 * @brief sets CMP inputs
 * @param p_input positive CMP input
 * @param m_input negative CMP input
 * @param n CMP's number
*/
void CMP_set_input (CMP_input p_input, CMP_input m_input, CMP_t n);
/**
 * @brief sets CMP outputs
 * @param output desired CMP output. See SIM_SOPT4 for more
 * @param n CMP's number
*/
void CMP_set_output(CMP_output output);
/**
 * @brief Returns the current value of the Analog Comparator output
 * @return an unsigned byte. The field will be reset to 0 after it is read 
*/
uint8_t CMP_get_output(CMP_t n);

/**
 * @brief sets CMP pin ports
 * @param Pin desired CMP pin
 * @param mux selected mux for the pin. Range 0-7
*/
void set_port_mux(pin_t Pin, uint8_t mux);

/**
 * @brief fully initializes CMP. No extra actions needed
 * @param n CMP's number
*/
void CMP_full_init(CMP_t n);
/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

#endif // _CMP_H_
