/***************************************************************************//**
  @file     SD.h
  @brief    SD Driver for K64F
  @authors  Micho
 ******************************************************************************/

#ifndef _MUSIC_
#define _MUSIC_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "ff.h"
#include "diskio.h"
#include "fsl_clock.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define N 50
#define M 16
#define BUFFER_SIZE (1024*M)
#define STRING_SPACE 64
#define TIMER_ID 1

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

uint8_t SD_Init (uint8_t songs[N][M]);

void PlaySong(char *name, uint8_t volumen);

uint32_t Mp3ReadId3V2Tag(FIL* pInFile, char* pszArtist, uint32_t unArtistSize, char* pszTitle, uint32_t unTitleSize);

void PauseSong(void);

void ResumeSong(void);

void getSongName(uint8_t SongName[1][M]);

/*******************************************************************************
 ******************************************************************************/

#endif // _MUSIC_