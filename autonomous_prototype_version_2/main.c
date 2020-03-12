#include<stdint.h>
#include <stdlib.h>
#include <string.h>
#include "Ublox.h"
#include "Drivers.h"
#include "FloatToString.h"
#include <math.h>
#include "compass.h"

//Yellow -> Most left -> E2
//Orange -> 2nd left -> E1
//Grey -> 2nd right -> E3
//Purple -> Most right -> E0

//float deg2rad(deg);
#define PI 3.141592653589793238462643383

double target_angle = 0;
double rotate = 0;
//double Target_Longitude = 0;
//double Target_Latitude = 0;
double heading = 0;
double difference = 0;
int m1 = 500;
int m2 = 500;
#define m_L 9000
#define m_R 9000
void PortE_Init(void);
void pwm_speed(void);
void backward(int motor1,int motor2);
void forward(int motor2,int motor1);
void right(int motor1,int motor2);
void left(int motor1,int motor2);
void stop();
float getDistanceFromLatLonInKm(float lat1,float lon1,float lat2,float lon2);
void bearing (double Target_Longitude,double Current_Longitude,double Current_Latitude, double Target_Latitude);

void main(void)
{
       char i, temp = 0;
       char str[20];
       double next_Latitude = 31.207067;
       double next_Longitude = 29.924060;
       float distance = 0, temp_distance = 0;
       //PortF_Init();
       PortE_Init();
       pwm_speed();
       UART0_Init();
       UART5_Init();
       I2C1_Init();
       QMC5883L_init();
       QMC5883L_setSamplingRate(50);
       delayMs(1);
       //UART0Tx('>');
       //delayMs(1000);


       while(1){

           //UART0Tx('f');
           i = 0;
           while(UART5Rx() != '\n');
           while(1){
               i = 0;
               while((buf1[i] = UART5Rx()) != '\n'){
                   //UART0Tx(buf1[i++]);
                   i++;
               }
               UART0Tx('\r');
               UART0Tx('\n');
               if(buf1[4] == 'G') break;
           }


           if(encode()){
           if (Sats_Num >= 5){
             I2C1_read(addr, 0x06,1, flag);
             while(((flag[0] & 0x01) == 0) || ((flag[0] & 0x04) == 1));
             heading = QMC5883L_readHeading();

             printStr("\rheading: \n");
             UART0Tx('\n');
             ftoa(heading,str,6);
             UART0Tx('\r');
             str[strlen(str)] = '\n';
             printStr(str);
             UART0Tx('\r');
             UART0Tx('\n');
             UART0Tx('\n');
             printStr("\rCurrent_Latitude: \n");
                          UART0Tx('\n');
                          ftoa(Current_Latitude,str,6);
                           UART0Tx('\r');
                           str[strlen(str)] = '\n';
                           printStr(str);
                           UART0Tx('\r');
                           UART0Tx('\n');
                           UART0Tx('\n');
             printStr("\rCurrent_Longitude: \n");
             UART0Tx('\n');
             ftoa(Current_Longitude,str,6);
             UART0Tx('\r');
             str[strlen(str)] = '\n';
             printStr(str);
             UART0Tx('\r');
             UART0Tx('\n');
             UART0Tx('\n');


             bearing(next_Longitude, Current_Longitude, Current_Latitude,  next_Latitude  );
             printStr("\rtargetAngle: \n");
             UART0Tx('\n');
             ftoa(target_angle,str,6);
              UART0Tx('\r');
              str[strlen(str)] = '\n';
              printStr(str);
              UART0Tx('\r');
              UART0Tx('\n');
              UART0Tx('\n');

             distance = getDistanceFromLatLonInm(Current_Latitude,Current_Longitude,next_Latitude,next_Longitude);
             printStr("\rdistance: \n");
             UART0Tx('\n');
             ftoa(distance,str,6);
              UART0Tx('\r');
              str[strlen(str)] = '\n';
              printStr(str);
              UART0Tx('\r');
              UART0Tx('\n');
              UART0Tx('\n');
              if(!temp) {
                  temp_distance = distance;
                  temp = 1;
              }

              m2 = (((distance - temp_distance)/(10-temp_distance))*5000)+10000;
              m1 = (((distance - temp_distance)/(10-temp_distance))*14000)+1000;

             //go forward
             rotate = heading - target_angle;
             difference = 180.0 - fabs(fabs(heading - target_angle) - 180.0);
             //\difference = abs(heading - target_angle);
             printStr("\rdifference: \n");
              UART0Tx('\n');
              ftoa(difference,str,6);
               UART0Tx('\r');
               str[strlen(str)] = '\n';
               printStr(str);
               UART0Tx('\r');
               UART0Tx('\n');
               UART0Tx('\n');

               if (difference <= 50){
                 forward(m1,m2);
               }
               else {
                 UART0Tx('p');
                 if (heading > target_angle){
                     UART0Tx('1');
                     UART0Tx(' ');
                     if (rotate < 180) left(m_L,m_R);
                     else right(m_L,m_R);
                 }
                 else {
                     UART0Tx('2');
                     UART0Tx(' ');
                     if ((-1*rotate) < 180) right(m_L,m_R);
                     else left(m_L,m_R);             }
            }
          if (distance <= 2)stop();

         }
         else {
             stop();
         }
        }
           else{
               stop();
           }
       }
}

float getDistanceFromLatLonInKm(float lat1,float lon1,float lat2,float lon2) {
  int R = 6371 * 1000; // Radius of the earth in km
  float dLat = deg2rad(lat2-lat1);  // deg2rad below
  float dLon = deg2rad(lon2-lon1);
  float a =
    sin(dLat/2) * sin(dLat/2) +
    cos(deg2rad(lat1)) * cos(deg2rad(lat2)) *
    sin(dLon/2) * sin(dLon/2);
  float c = 2 * atan2(sqrt(a), sqrt(1-a));
  float d = R * c ; // Distance in km
  return (d * 1000);
}

void PortE_Init(void){
    SYSCTL_RCGCGPIO_R |= 0x10;
    GPIO_PORTE_DIR_R = 0x0F;
    GPIO_PORTE_DEN_R = 0x0F;
}

void pwm_speed(void){
    //int speed;
    /* Enable Peripheral Clocks */
          SYSCTL_RCGCPWM_R |= 2; /* enable clock to PWM1 */
          SYSCTL_RCGCGPIO_R |= 0x20; /* enable clock to PORTF */
          SYSCTL_RCC_R &= ~0x00100000; /* no pre-divide for PWM clock */
          /* Enable port PF3 for PWM1 M1PWM7 */
          GPIO_PORTF_AFSEL_R |= 0x08; /* PF3 uses alternate function */
          GPIO_PORTF_AFSEL_R |= 0x02;
          GPIO_PORTF_PCTL_R &= ~0x0000F0F0; /* make PF3 PWM output pin */
          GPIO_PORTF_PCTL_R |= 0x00005050;
          GPIO_PORTF_DEN_R |= 0x08; /* pin digital */
          GPIO_PORTF_DEN_R |= 0X02;
          PWM1_3_CTL_R = 0; /* stop counter */
          PWM1_2_CTL_R = 0;
          PWM1_3_GENB_R = 0x0000008C; /* M1PWM7 output set when reload, */
          /* clear when match PWMCMPA */
          PWM1_2_GENB_R = 0x0000008C;
          PWM1_3_LOAD_R = 16000; /* set load value for 1kHz (16MHz/16000) */
          PWM1_2_LOAD_R = 16000;
          PWM1_3_CTL_R = 1; /* start timer */
          PWM1_2_CTL_R = 1;
          PWM1_ENABLE_R |= 0xA0; /* start PWM1 ch7 */
}


void forward(int motor2,int motor1){
    UART0Tx('f');
    GPIO_PORTE_DATA_R &= ~(0X0F);
    GPIO_PORTE_DATA_R |= 0X09;
    PWM1_3_CMPA_R = motor1;
    PWM1_2_CMPA_R = motor2;
}
void backward(int motor1,int motor2){
    UART0Tx('b');
    GPIO_PORTE_DATA_R &= ~(0X0F);
    GPIO_PORTE_DATA_R |= 0X0A;
    PWM1_3_CMPA_R = motor1;
    PWM1_2_CMPA_R = motor2;
}
void left(int motor1,int motor2){
    UART0Tx('l');
    GPIO_PORTE_DATA_R &= ~(0X0F);
    GPIO_PORTE_DATA_R |= 0X01;
    PWM1_3_CMPA_R = motor1;
    PWM1_2_CMPA_R = motor2;
}

void right(int motor1,int motor2){
    UART0Tx('r');
    GPIO_PORTE_DATA_R &= ~(0X0F);
    GPIO_PORTE_DATA_R |= 0X08;
    PWM1_3_CMPA_R = motor1;
    PWM1_2_CMPA_R = motor2;
}

void stop(){
    GPIO_PORTE_DATA_R &= ~(0X0F);

    PWM1_3_CMPA_R = 0;
    PWM1_2_CMPA_R = 0;
}

/*float deg2rad(deg) {
  return deg * (M_PI/180);
}*/

void bearing (double Target_Longitude,double Current_Longitude,double Current_Latitude, double Target_Latitude ){
    double y = sin(Target_Longitude-Current_Longitude) * cos(Target_Latitude);
      double x = cos(Current_Latitude) * sin(Target_Latitude);
      x -= sin(Current_Latitude) * cos(Target_Latitude) * cos((Target_Longitude-Current_Longitude));
      double brng = atan2(y, x);
      brng = brng * (180/PI);
      if (brng < 0){
          brng = fmod((brng + 360),360); // brng = (brng + 360) % 360
      }
      target_angle = brng;
}
/*
void direction(){


    if (rotate <= 10){

    }
}*/
