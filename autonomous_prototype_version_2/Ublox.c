#include "Ublox.h"

double Current_Longitude;
double Current_Latitude;
char Sats_Num;
char Validity;

bool encode()
{
        bool ret = process_buf();
        memset(buf1, '\0', 120);
        return ret;
}

bool process_buf(void)
{

    if(!check_checksum()) //if checksum is bad
    {
        return false; //return
    }

    //otherwise, what sort of message is it
    if(strncmp(buf1, "$GNGGA", 6) == 0)
    {
        read_gga();
    }
    else
    {
        return false;
    }

    return true;
}


bool check_checksum(void)
{
    char i;
    if (buf1[strlen(buf1)-5] == '*')
    {
        char sum = parse_hex(buf1[strlen(buf1)-4]) * 16;
        sum += parse_hex(buf1[strlen(buf1)-3]);

    for (i=1; i < (strlen(buf1)-5); i++)
            sum ^= buf1[i];
        if (sum != 0){
            printStr("WRONG1\n");
            UART0Tx('\n');
            return false;
        }
        return true;
    }
    printStr("WRONG2\n");
    UART0Tx('\n');
    return false;
}


char parse_hex(char c)
{
    if (c < '0')
        return 0;
    if (c <= '9')
        return c - '0';
    if (c < 'A')
        return 0;
    if (c <= 'F')
        return (c - 'A')+10;

    return 0;
}

void read_gga(void)
{
    printStr("GGA: \n");
    UART0Tx('\n');
    char counter = 0;
    char *token = strtok(buf1,",");

    while(token[counter] != NULL)
    {
        switch(counter)
        {
					case 2: //Latitude
					{
							double llat = atof(token);
							int ilat = llat/100;
							double mins = fmod(llat, 100);
							Current_Latitude = ilat + (mins/60);
							break;
					}

					case 3: //North/South
					{
							if(token[0] == 'S')
									Current_Latitude = -Current_Latitude;
							break;
					}

					case 4: //Longitude
					{
							double llong = atof(token);
							int ilat = llong/100;
							double mins = fmod(llong, 100);
							Current_Longitude = ilat + (mins/60);
							break;
					}
					case 5: //East/West
					{
							if(token[0] == 'W')
									Current_Longitude = -Current_Longitude;
							break;
					}

					case 6:
					{
							Validity = atoi(token);
							break;
					}

					case 7:
					{
							Sats_Num = atoi(token);
							break;
					}

        }
				token = strtok(NULL,",");
        counter++;
    }
}

void printStr(char *str){
    char i;
    for(i=0;str[i]!='\n';i++){
        UART0Tx(str[i]);
    }
    //UART0Tx('\n');
}

