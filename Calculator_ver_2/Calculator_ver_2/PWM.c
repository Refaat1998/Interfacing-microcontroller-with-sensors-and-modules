/*
 * PWM.c
 *
 *  Created on: Oct 12, 2019
 *      Author: 20120
 */
#include "Std_types.h"
#include "Register.h"
#include "utils.h"
#include "PWM_Priv.h"
#include "PWM.h"


void PWM0_Init(void)
{
	//  Wave generator (WGM01:0)
#if PWM_Mode == Fast_Mode

	SETBIT(TCCR2,3);
	SETBIT(TCCR2,6);
#elif PWM_Mode == PhaseCorrect_Mode

#error Will be added later

#else

#error Invalid I/P Choose (Fast_Mode / PhaseCorrect_Mode)

#endif
	//  Compare match O/P COM01:0:
#if PWM_Output_Mode	==	Non_Invert

	CLRBIT(TCCR2,4);
	SETBIT(TCCR2,5);

#elif PWM_Output_Mode	==	Invert

	SETBIT(TCCR2,4);
	SETBIT(TCCR2,5);

#else

#error Invalid I/P Choose (Invert / Non-Invert)

#endif


	// Determine the PreScaler
#if PWM_Freq	==	_61_Hz
	SETBIT(TCCR2,0);
	CLRBIT(TCCR2,1);
	SETBIT(TCCR2,2);
#elif PWM_Freq	==	_244_Hz
	CLRBIT(TCCR2,0);
	SETBIT(TCCR2,1);
	SETBIT(TCCR2,2);
#elif PWM_Freq	==	_1_KHz
	SETBIT(TCCR2,0);
	SETBIT(TCCR2,1);
	CLRBIT(TCCR2,2);
#elif PWM_Freq	==	_7_KHz
	CLRBIT(TCCR2,0);
	SETBIT(TCCR2,1);
	CLRBIT(TCCR2,2);
#else
#error Invalid I/P Choose (_61_Hz / _244_Hz / _1_KHz / _7_KHz)
#endif
	// Set OC0 Pin as Output

}


void PWM0_SetDutyCycle(u8 DC)
{
if (DC>-1 && DC<101)
{
	OCR2= (u8) ((DC*255)/100);
}
else
{
	OCR2=0;
}

}

void PWM0_Stop(){
		CLRBIT(TCCR2,0);
		CLRBIT(TCCR2,1);
		CLRBIT(TCCR2,2);
}


