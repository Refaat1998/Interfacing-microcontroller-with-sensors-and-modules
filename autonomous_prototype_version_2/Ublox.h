#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

bool encode();
void read_gga(void);
char parse_hex(char c);
bool check_checksum(void);
bool process_buf(void);



char buf1[120];


void UART0Tx(volatile double c);
void printStr(char *str);
char UART5Rx(void);

extern double Current_Longitude;
extern double Current_Latitude;
extern char Sats_Num;
extern char Validity;
