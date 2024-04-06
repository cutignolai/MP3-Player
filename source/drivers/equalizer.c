/***************************************************************************//**
  @file     equalizer.h
  @brief    Equalizer, filter and vumeter functions
  @author   Cuty
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include "../CMSIS/arm_math.h"
#include "equalizer.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define MAX_SIGNAL 8000000


static bool Equalizer_Status;	// Flag que indica si el ecualizador esta encendido o no


/**
 *  VAMOS A UTILIZAR LA SIGUIENTE LIBRERIA PARA REALIZAR EL ECUALIZADOR
 * 	https://arm-software.github.io/CMSIS_5/DSP/html/group__BiquadCascadeDF1__32x64.html
 * 	https://www.keil.com/pack/doc/CMSIS_Dev/DSP/html/group__BiquadCascadeDF1.html#gad0b680d1c529d941e939d265b3c6ae59
 * 
 * Utilizaremos un Biquad Cascade Filter. Un filtro lineal recursivo que contiene dos polos y dos ceros. 
 * 
 * @brief  Funcion para inicializar el Biquad Cascade Filter.
 * @param[in,out] S          Instancia de Biquad Cascade Structure.
 * @param[in]     numStages  number of 2nd order stages in the filter.
 * @param[in]     pCoeffs    points to the filter coefficients.
 * @param[in]     pState     points to the state buffer.
 * 
 *      void arm_biquad_cascade_df1_init_f32(
 *          arm_biquad_casd_df1_inst_f32 * S, 
 *          uint8_t numStages, float32_t * pCoeffs, 
 *          float32_t * pState);
 */


static float32_t pState[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static float32_t pCoeffs [5*4];






static float32_t equCoeffs_filter [12][5] =   {{1.00453968207833,-2,0.995917140680743,1.97554429635406,-0.975995533152126},     // 150Hz    -9dB
											{1.007695793510805,-2,0.992761029248211 ,1.970195284624802,-0.970645299647719 },    // 150Hz    -6dB
											{1.021740027305337,-2,0.978716795453736 ,1.937205750397515,-0.937648230235409 },    // 150Hz    -3.5dB
											{1.015730293762013,-2,0.989355301752624 ,1.923612924125309,-0.928504282754724},     // 500Hz    -9dB
											{1.025384340054628,-2,0.979701255460009 ,1.908182701493999,-0.913034824187912},     // 500Hz    -6dB
											{1.068343432627087,-2,0.936742162887549 ,1.816536937638274,-0.821156023689386},     // 500Hz    -3.5dB
											{1.063771500843777,-2,0.982787728876055 ,1.758394402069101,-0.799329146521103 },    // 1500Hz   -9dB
											{1.093414015360886,-2,0.953145214358947 ,1.719370914163331,-0.759397206846395 },    // 1500Hz   -6dB
											{1.225318865060547,-2,0.821240364659285 ,1.508775857076812,-0.543899577939500 },    // 1500Hz   -3dB
											{1.315471063170089,-2,1.059958207403383 ,1.292172328674553,-0.534731986079310 },    // 4000Hz   -9dB
											{1.408996510952285,-2,0.966432759621188 ,1.227573744668495,-0.458007302436515},     // 4000Hz   -6dB
											{1.825171060708643,-2,0.550258209864829 ,0.933924963748510,-0.109236347703740}};    // 4000Hz   -3dB

static arm_biquad_casd_df1_inst_f32 equ;


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void On_Off_equalizer(bool status)
{
	Equalizer_Status = status;
}

int GetOnOffEq()
{
    return Equalizer_Status;
}

void setUpFilter(int N_Frequency, int GaindB) {
	int i;

	/**
	 * 	     b0 + b1.z^{-1} + b2.z^{-2}
	 * H(z)= ---------------------------
	 * 	      1 + a1.z^{-1} + a2.z^{-2}
	*/

	if (GaindB > 0) {
		i = (N_Frequency*3) + (9/GaindB) - 1; // Asumo ganancia positiva
		pCoeffs[N_Frequency*5+0] = equCoeffs_filter [i][0] ; // b0
		pCoeffs[N_Frequency*5+1] = equCoeffs_filter [i][1] ; // b1
		pCoeffs[N_Frequency*5+2] = equCoeffs_filter [i][2] ; // b2

		pCoeffs[N_Frequency*5+3] = equCoeffs_filter [i][3] ; // a1
		pCoeffs[N_Frequency*5+4] = equCoeffs_filter [i][4] ; // a2
	}
	else {
		pCoeffs[N_Frequency*5] = 1;		// b0
		pCoeffs[N_Frequency*5 + 1] = 0;	// b1
		pCoeffs[N_Frequency*5 + 2] = 0;	// b2 
		
		pCoeffs[N_Frequency*5 + 3] = 0;	// a1
		pCoeffs[N_Frequency*5 + 4] = 0; // a2
	}

	/**
	 * Higher order filters are realized as a cascade of second order sections. numStages refers to the number of second order stages used. 
	 * For example, an 8th order filter would be realized with numStages=4 second order stages.
	*/
	arm_biquad_cascade_df1_init_f32(
		&equ,
		4, 
		pCoeffs, 
		pState);
}



void setUpCascadeFilter(char* GaindB) 
{
	int i;
	for(i = 0; i<4; i++) {
		setUpFilter(i, GaindB[i]-'0');
	}
}
