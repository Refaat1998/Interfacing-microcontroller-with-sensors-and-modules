/*
 * PWM.h
 *
 *  Created on: Oct 12, 2019
 *      Author: 20120
 */

#ifndef PWM_H_
#define PWM_H_

/*
 * PWM_Mode: Fast_Mode / PhaseCorrect_Mode
 * Only Fast_Mode available till now
 */
#define PWM_Mode    Fast_Mode

/*
 *	PWM_Freq: _61_Hz / _244_Hz / _1_KHz / _7_KHz
 */
#define PWM_Freq	_244_Hz

/*
 * 	PWM_Output_Mode: Non-Invert / Invert
 */
#define PWM_Output_Mode		Non_Invert

//Functions

void PWM0_Init(void);

/*
 * DC: range from (1->100) percentage
 */
void PWM0_SetDutyCycle(u8 DC);

void PWM0_Stop();

#endif /* PWM_H_ */
