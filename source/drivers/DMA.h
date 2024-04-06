/***************************************************************************//**
  @file     DMA.h
  @author   Ignacio Cutignola
 ******************************************************************************/

#ifndef DMA_H_
#define DMA_H_

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define FTM0CH0       20
#define FTM0CH1       21
#define FTM0CH2       22
#define FTM0CH3       23
#define FTM0CH4       24
#define FTM0CH5       25
#define FTM0CH6       26
#define FTM0CH7       27
#define DMADAC0       45
#define DMADAC1       46
#define DMAALWAYS63   63	

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef enum {
	DMA_0,
	DMA_1,
} DMA_n;

typedef struct DMA_config
{
  uint8_t  channel;
  uint16_t* source_buffer;
  uint16_t* destination_buffer;
  uint8_t source_offset;
  uint8_t transfer_size;
  uint32_t source_full_size;
  uint32_t source_unit_size;
  DMA_request_t request_source;
}DMA_config_t;




/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void DMA_Config(DMA_config_t config);

void DMA_SetCallback(uint8_t channel, void(*funcallback)(void));

uint32_t DMA_GetRemainingMajorLoopCount(uint32_t channel);

#endif /* DMA_H_ */
