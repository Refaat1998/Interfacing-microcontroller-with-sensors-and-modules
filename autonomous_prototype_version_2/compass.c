#include "compass.h"
 int16_t xhigh, xlow;
 int16_t yhigh, ylow;
 uint8_t addr;
 uint8_t mode;
 uint8_t rate;
 uint8_t range;
 uint8_t oversampling;
char timeDateReadBack[7];
char flag[2];
void QMC5883L_init() {
  /* This assumes the wire library has been initialized. */
  addr = QMC5883L_ADDR;
  oversampling = QMC5883L_CONFIG_OS512;
  range = QMC5883L_CONFIG_2GAUSS;
  rate = QMC5883L_CONFIG_50HZ;
  mode = QMC5883L_CONFIG_CONT;
  QMC5883L_reset();
}

void QMC5883L_reset()
{
  I2C1_byteWrite(addr,QMC5883L_RESET,0x01);
  QMC5883L_reconfig();
}

void QMC5883L_reconfig()
{
  I2C1_byteWrite(addr,QMC5883L_CONFIG,oversampling|range|rate|mode);
  I2C1_byteWrite(addr,0x0A,0x41);

}

void QMC5883L_setOversampling( int x )
{
  switch(x) {
    case 512:
      oversampling = QMC5883L_CONFIG_OS512;
      break;
    case 256:
      oversampling = QMC5883L_CONFIG_OS256;
      break;
    case 128:
      oversampling = QMC5883L_CONFIG_OS128;
      break;
    case 64:
      oversampling = QMC5883L_CONFIG_OS64;
      break;
  }
  QMC5883L_reconfig();
}

void QMC5883L_setSamplingRate( int x )
{
  switch(x) {
    case 10:
      rate = QMC5883L_CONFIG_10HZ;
      break;
    case 50:
      rate = QMC5883L_CONFIG_50HZ;
      break;
    case 100:
      rate = QMC5883L_CONFIG_100HZ;
      break;
    case 200:
      rate = QMC5883L_CONFIG_200HZ;
      break;
  }
  QMC5883L_reconfig();
}

int QMC5883L_readHeading()
{
    //signed char timeDateReadBack[7];
  // static int last_heading = 0;
    static int heading = 0;
    //int temp = 0;
    char test;
  int16_t x, y, z;
  test = I2C1_read(addr, 0x00, 6, timeDateReadBack);
  if (test == 0){
 // if(!readRaw(&x,&y,&z,&t)) return 0;
  x = timeDateReadBack[0] | (timeDateReadBack[1] << 8); //MSB  x
  //x |= timeDateReadBack[1] << 8; //LSB  x
  y = timeDateReadBack[2] | (timeDateReadBack[3] << 8); //MSB  z
//y |= timeDateReadBack[3] << 8; //LSB z
  z = timeDateReadBack[4] | (timeDateReadBack[5] << 8); //MSB y
 //z |= timeDateReadBack[5] << 8;


  /* Update the observed boundaries of the measurements */

  if(x<xlow) xlow = x;
  if(x>xhigh) xhigh = x;
  if(y<ylow) ylow = y;
  if(y>yhigh) yhigh = y;

  /* Bail out if not enough data is available. */

  if( xlow==xhigh || ylow==yhigh ) return 0;

  /* Recenter the measurement by subtracting the average */

  x -= (xhigh+xlow)/2;
  y -= (yhigh+ylow)/2;

  /* Rescale the measurement to the range observed. */

  float fx = (float)x/(xhigh-xlow);
  float fy = (float)y/(yhigh-ylow);
  //last_heading = heading;
  heading = 180.0*atan2(fy,fx)/M_PI;
  if(heading<=0) heading += 360;
  }
      return heading;
}
