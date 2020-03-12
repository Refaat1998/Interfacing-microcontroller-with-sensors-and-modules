/*
 * compass.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: Abd alrhman
 */
#include "stdint.h"
#include "tm4c123gh6pm.h"
#include <math.h>
#include "FloatToString.h"
#include <String.h>

#ifndef COMPASS_H_
#define COMPASS_H_
void QMC5883L_init();
void QMC5883L_reset();
void QMC5883L_reconfig();
void QMC5883L_setOversampling( int x );
void QMC5883L_setSamplingRate( int x );
int QMC5883L_readHeading();


#define PI 3.141592653589793238462643383
extern int16_t xhigh, xlow;
extern int16_t yhigh, ylow;
extern uint8_t addr;
extern uint8_t mode;
extern uint8_t rate;
extern uint8_t range;
extern uint8_t oversampling;
extern char timeDateReadBack[7];
extern char flag[2];
/* The default I2C address of this chip */
#define QMC5883L_ADDR 0x0D

/* Register numbers */
#define QMC5883L_X_LSB 0
#define QMC5883L_X_MSB 1
#define QMC5883L_Y_LSB 2
#define QMC5883L_Y_MSB 3
#define QMC5883L_Z_LSB 4
#define QMC5883L_Z_MSB 5
#define QMC5883L_STATUS 6
#define QMC5883L_TEMP_LSB 7
#define QMC5883L_TEMP_MSB 8
#define QMC5883L_CONFIG 9
#define QMC5883L_CONFIG2 10
#define QMC5883L_RESET 11
#define QMC5883L_RESERVED 12
#define QMC5883L_CHIP_ID 13

/* Bit values for the STATUS register */
#define QMC5883L_STATUS_DRDY 1
#define QMC5883L_STATUS_OVL 2
#define QMC5883L_STATUS_DOR 4

/* Oversampling values for the CONFIG register */
#define QMC5883L_CONFIG_OS512 0b00000000
#define QMC5883L_CONFIG_OS256 0b01000000
#define QMC5883L_CONFIG_OS128 0b10000000
#define QMC5883L_CONFIG_OS64  0b11000000

/* Range values for the CONFIG register */
#define QMC5883L_CONFIG_2GAUSS 0b00000000
#define QMC5883L_CONFIG_8GAUSS 0b00010000

/* Rate values for the CONFIG register */
#define QMC5883L_CONFIG_10HZ   0b00000000
#define QMC5883L_CONFIG_50HZ   0b00000100
#define QMC5883L_CONFIG_100HZ  0b00001000
#define QMC5883L_CONFIG_200HZ  0b00001100

/* Mode values for the CONFIG register */
#define QMC5883L_CONFIG_STANDBY 0b00000000
#define QMC5883L_CONFIG_CONT    0b00000001




#endif /* COMPASS_H_ */
