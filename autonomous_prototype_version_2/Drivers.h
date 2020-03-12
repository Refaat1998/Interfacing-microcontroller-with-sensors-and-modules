#include"tm4c123gh6pm.h"

void delayMs(int n);
void PortF_Init(void);
void UART0_Init(void);
void UART0Tx(volatile double c);
char UART5Rx(void);
void UART5_Init(void);
float getDistanceFromLatLonInm(float lat1,float lon1,float lat2,float lon2);
float deg2rad(float deg);
void I2C1_Init(void);
char I2C1_read(int slaveAddr, char memAddr, int byteCount, char* data);
char I2C1_byteWrite(int slaveAddr, char memAddr, char data);
