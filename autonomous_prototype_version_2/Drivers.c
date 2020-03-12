#include "Drivers.h"
#include <math.h>

void PortF_Init(void){
  volatile unsigned int delay;
  SYSCTL_RCGCGPIO_R |= 0x00000020;     // 1) Activate clock for Port F
  delay = SYSCTL_RCGCGPIO_R;           // Allow time for clock to start
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) Unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // Allow changes to PF4-0
//Only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PortF
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 in, PF3-1 out
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x0E;// 7) enable digital I/O on PF4-0
  GPIO_PORTF_DATA_R = 0;
}


/* delay n milliseconds (16 MHz CPU clock) */
void delayMs(int n)
{
int i, j;
for(i = 0 ; i < n; i++)
for(j = 0; j < 3180; j++)
{} /* do nothing for 1 ms */
}

/* UART0 Receive */
/* This function waits until a character is received then returns it. */
char UART5Rx(void)
{char c;
while((UART5_FR_R & 0x10) != 0); /* wait until the buffer is not empty */
c = UART5_DR_R;/* read the received data */
GPIO_PORTF_DATA_R = c << 1;

return c; /* and return it */
}

void UART0_Init(void){
       SYSCTL_RCGCUART_R |= 0x01;
       SYSCTL_RCGCGPIO_R |= 1;
       delayMs(1);

       UART0_CTL_R &= 0x110;
       UART0_IBRD_R = 104;
       UART0_FBRD_R = 11;
       UART0_CC_R  = 0;
       UART0_LCRH_R = 0x60;
       UART0_CTL_R = 0x301;
       GPIO_PORTA_DEN_R = 0x03;
       GPIO_PORTA_AFSEL_R = 0x03;
       GPIO_PORTA_PCTL_R = 0x11;

       delayMs(1);
}

void UART5_Init(void){
    SYSCTL_RCGCUART_R |= 0x20; /* provide clock to UART5 */
    SYSCTL_RCGCGPIO_R |= 0x10; /* Enable clock to PORTE */
    /* UART5 initialization */
    UART5_CTL_R = 0; /* disable UART5 */
    UART5_IBRD_R = 104; /* 16MHz/16=1MHz, 1MHz/104=9600 baud rate */
    UART5_FBRD_R = 11; /* fraction part, see Example 4-4 */
    UART5_CC_R = 0; /* use system clock */
    UART5_LCRH_R = 0x60; /* 8-bit, no parity, 1-stop bit */
    //UART5_IM_R |= 0x0010;
    UART5_CTL_R = 0x301; /* enable UART5, TXE, RXE */
    /* UART5 TX5 and RX5 use PE5 and PE4. Set them up. */
    GPIO_PORTE_DEN_R |= 0x30; /* make PE5, PE4 as digital */
    GPIO_PORTE_AFSEL_R |= 0x30; /* use PE5, PE4 alternate function */
    GPIO_PORTE_PCTL_R |= 0x00110000; /* configure PE5, PE4 for UART5 */
}

void UART0Tx(volatile double c)
{
while((UART0_FR_R  & 0x20) != 0); /* wait until Tx buffer not full */
UART0_DR_R = c; /* before giving it another byte */
}

float getDistanceFromLatLonInm(float lat1,float lon1,float lat2,float lon2) {
  int R = 6371; // Radius of the earth in km
  float dLat = deg2rad(lat2-lat1);  // deg2rad below
  float dLon = deg2rad(lon2-lon1);
  float a =
    sin(dLat/2) * sin(dLat/2) +
    cos(deg2rad(lat1)) * cos(deg2rad(lat2)) *
    sin(dLon/2) * sin(dLon/2);
  float c = 2 * atan2(sqrt(a), sqrt(1-a));
  float d = R * c * 1000 ; // Distance in m
  return d;
}

float deg2rad(float deg) {
  return deg * (M_PI/180);
}

void I2C1_Init(void){
    SYSCTL_RCGCI2C_R  |= 0x02; /* enable clock to I2C1 */
    SYSCTL_RCGCGPIO_R |= 0x01; /* enable clock to GPIOA */

    /* PORTA 7, 6 for I2C1 */
    GPIO_PORTA_AFSEL_R |= 0xC0; /* PORTA 7, 6 for I2C1 */
    GPIO_PORTA_PCTL_R &= ~0xFF000000; /* PORTA 7, 6 for I2C1 */
    GPIO_PORTA_PCTL_R |= 0x33000000;
    GPIO_PORTA_DEN_R |= 0xC0; /* PORTA 7, 6 as digital pins */
    GPIO_PORTA_ODR_R |= 0x80; /* PORTA 7 as open drain */

    I2C1_MCR_R |= 0x10; /* master mode */
    I2C1_MTPR_R = 1; /* 100 kHz @ 16 MHz */

}

static int I2C_wait_till_done(void)
{
while(I2C1_MCS_R & 0x01);/* wait until I2C master is not busy */
return I2C1_MCS_R & 0x0E; /* return I2C error code */
}


/* Read memory */
/* read: S-(saddr+w)-ACK-maddr-ACK-R-(saddr+r)-ACK-data-ACK-data-ACK-...-data-NACK-P */
char I2C1_read(int slaveAddr, char memAddr, int byteCount, char* data)
{
    char error;
    if (byteCount <= 0)
        return -1; /* no read was performed */

    /* send slave address and starting address */
    I2C1_MSA_R = slaveAddr << 1;
    I2C1_MDR_R  = memAddr;
    I2C1_MCS_R = 3;/* S-(saddr+w)-ACK-maddr-ACK */
    error = I2C_wait_till_done();
    if (error){
        //UART0Tx('W');
    return error;}
    /* to change bus from write to read, send restart with slave addr */
    I2C1_MSA_R = (slaveAddr << 1) + 1; /* restart: -R-(saddr+r)-ACK */
    if (byteCount == 1){ /* if last byte, don't ack */
        I2C1_MCS_R = 7;/* -data-NACK-P */
        //UART0Tx('E');
    }
    else /* else ack */
        I2C1_MCS_R = 0xB; /* -data-ACK- */
    error = I2C_wait_till_done();

    if (error) return error;

    *data++ = I2C1_MDR_R; /* store the data received */
    if (--byteCount == 0) /* if single byte read, done */
    {
       // UART0Tx('A');
        while(I2C1_MCS_R & 0x40); /* wait until bus is not busy */
        return 0; /* no error */
    }
    /* read the rest of the bytes */
    while (byteCount > 1)
    {
        I2C1_MCS_R = 9; /* -data-ACK- */
        error = I2C_wait_till_done();
        //UART0Tx('r');
        if (error){
            //UART0Tx('f');
            return error;
        }
        byteCount--;
        *data++ = I2C1_MDR_R;/* store data received */

    }
    I2C1_MCS_R = 5; /* -data-NACK-P */
    error = I2C_wait_till_done();
    *data = I2C1_MDR_R;/* store data received */
    while(I2C1_MCS_R & 0x40); /* wait until bus is not busy */
    return 0; /* no error */
}










char I2C1_byteWrite(int slaveAddr, char memAddr, char data)
{
char error;
/* send slave address and starting address */
I2C1_MSA_R= slaveAddr << 1;
I2C1_MDR_R = memAddr;
I2C1_MCS_R = 3; /* S-(saddr+w)-ACK-maddr-ACK */
error = I2C_wait_till_done(); /* wait until write is complete */
if (error) return error;
/* send data */
I2C1_MDR_R = data;
I2C1_MCS_R = 5; /* -data-ACK-P */
error = I2C_wait_till_done(); /* wait until write is complete */
while(I2C1_MCS_R & 0x40); /* wait until bus is not busy */
error =I2C1_MCS_R & 0xE;
if (error) return error;
return 0; /* no error */
}

