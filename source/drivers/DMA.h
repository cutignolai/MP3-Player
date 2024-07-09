/***************************************************************************//**
  @file     DMA.h
  @author   Ignacio Cutignola
 ******************************************************************************/

#ifndef DMA_H_
#define DMA_H_

#include <stdint.h>
#include <stdbool.h>
#include "MK64F12.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define DMA_TEST_PIN		PORTNUM2PIN(PA, 2)
//#define DMA_DEBUG

#define DMA_n				16

/* DMA Request Sources */
#define DMA_UART0_RX        2
#define DMA_UART0_TX        3
#define DMA_UART1_RX        4
#define DMA_UART1_TX        5
#define DMA_UART2_RX        6
#define DMA_UART2_TX        7
#define DMA_UART3_RX        8
#define DMA_UART3_TX        9
#define DMA_UART4_TXRX      10
#define DMA_UART5_TXRX      11
#define DMA_I2S0_RX         12
#define DMA_I2S0_TX         13
#define DMA_SPI0_RX         14
#define DMA_SPI0_TX         15
#define DMA_SPI1_TXRX       16
#define DMA_SPI2_TXRX       17
#define DMA_I2C0            18
#define DMA_I2C1            19
#define DMA_I2C2            19
#define DMA_FTM0CH0         20
#define DMA_FTM0CH1         21
#define DMA_FTM0CH2         22
#define DMA_FTM0CH3         23
#define DMA_FTM0CH4         24
#define DMA_FTM0CH5         25
#define DMA_FTM0CH6         26
#define DMA_FTM0CH7         27
#define DMA_FTM1CH0         28
#define DMA_FTM1CH1         29
#define DMA_FTM2CH0         30
#define DMA_FTM2CH1         31
#define DMA_FTM3CH0         32
#define DMA_FTM3CH1         33
#define DMA_FTM3CH2         34
#define DMA_FTM3CH3         35
#define DMA_FTM3CH4         36
#define DMA_FTM3CH5         37
#define DMA_FTM3CH6         38
#define DMA_FTM3CH7         39
#define DMA_ADC0            40
#define DMA_ADC1            41
#define DMA_CMP0            42
#define DMA_CMP1            43
#define DMA_CMP2            44
#define DMA_DAC0            45
#define DMA_DAC1            46
#define DMA_CMT             47
#define DMA_PDB             48
#define DMA_PA              49
#define DMA_PB              50
#define DMA_PC              51
#define DMA_PD              52
#define DMA_PE              53
#define DMA_1588Timer0      54
#define DMA_1588Timer1      55
#define DMA_1588Timer2      56
#define DMA_1588Timer3      57
#define DMA_MUX0            58
#define DMA_MUX1            59
#define DMA_MUX2            60
#define DMA_MUX3            61
#define DMA_MUX4            62
#define DMA_MUX5            63

#define DMADAC0       DMA_DAC0
#define DMADAC1       DMA_DAC1
#define DMAALWAYS63   DMA_MUX5	

#define DMA_BUFFER_SIZE(x)      (1<<(x))

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef enum {DMA_0, DMA_1, DMA_2, DMA_3, DMA_4, DMA_5, DMA_6, DMA_7, DMA_8, DMA_9, DMA_10, DMA_11, DMA_12, DMA_13, DMA_14, DMA_15} DMAn_t;

typedef struct DMA_config
{
  uint8_t  channel;
  dma_request_source_t request_source;
  uint16_t* source_buffer;
  uint16_t* destination_buffer;
  bool trigger;
  int8_t source_offset;
  int8_t destination_offset;
  uint8_t transfer_size;
  uint8_t source_module;
  uint8_t destination_module;
  uint8_t minor_loop_count;
  uint32_t mayor_loop_count;
#ifdef DMA_MINOR_LOOP_OFFSET
  bool ml_offset_s;				// Si agregarle un offset a source o destination después de cada minor loop
  bool ml_offset_d;
  uint32_t ml_offset;
#endif
  int32_t mayor_loop_offset_s;	// Define el offset a ser sumado al source address luego de cada mayor loop, es SIGNADO
  int32_t mayor_loop_offset_d;
  bool scatter_gather;			// Habilita el cambio a un TCD distinto luego de finalizar un mayor loop. Se cargará el TCD al que apunte mayor_loop_offset_d
#ifdef DMA_MINOR_LINK
  bool minor_link;					// Define si linkear a otro canal al finalizar el minor loop
  uint8_t minor_link_ch;
#endif
#ifdef DMA_MAYOR_LINK
  bool mayor_link;					// Define si linkear a otro canal al finalizar el mayor loop
  uint8_t mayor_link_ch;
#endif
} DMA_config_t;

// DMA transfer size
typedef enum {
	DMA_SIZE_1BYTE				= 1,
	DMA_SIZE_2BYTES				= 2,
	DMA_SIZE_4BYTES				= 4,
	DMA_SIZE_16BYTE_BURST		= 16,
	DMA_SIZE_32BYTE_BURST		= 32,
} DMA_size_t;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void DMA_Config(DMA_config_t config);

void DMA_EnableChannel(uint8_t channel);

void DMA_DisableChannel(uint8_t channel);

bool DMA_IsTransfer(uint8_t channel);

void DMA_SetCallback(uint8_t channel, void(*funcallback)(void));

void DMA_SetErrorCallback(DMAn_t channel, void(*funcallback)(void));

uint32_t DMA_GetRemainingMajorLoopCount(uint32_t channel);

#endif /* DMA_H_ */
