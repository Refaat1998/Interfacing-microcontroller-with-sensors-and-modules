/*
 * RTOS_LAB1_ATMEGA32.c
 *
 * Created: 1/27/2014 12:22:11 AM
 *  Author: Islam
 */ 

#include <avr/io.h>
#include "lcd_4bit.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "KeyPad_int.h"
#include "KeyPad_priv.h"
#include "PWM.h"
#define F_CPU    8000000
#include <util/delay.h>

//#include "FreeRTOSConfig.h"

/* Define Tasks Priorities */
#define  TASK1_PRIORITY (1)
#define  TASK2_PRIORITY (2)
#define  TASK3_PRIORITY (3)
#define  TASK4_PRIORITY (4)

 /*tasks codes prototypes */
void task1_code(void*pvParamters);
void task2_code(void*pvParamter);
void Normal_op(void*pvParamter);
void display_res(void*pvParamter);
void Blinking(void*pvParamter);
//void task2_code(void*pvParamter);

xTaskHandle  welcome;
xTaskHandle  task2;
xTaskHandle  Normal;
xTaskHandle display;
xTaskHandle led;

int result = 0;
char res[20] = {0};
unsigned char op_value = 0; 
char operations[20] = {0};
unsigned char ind_op = 0;
int num1 = 0;
int num2 = 0;	
	
int main(void)
{
	/*initialize LCD*/
	lcd_init();	
	lcd_clrScreen();	
	
	/*Creat tasks*/
	xTaskCreate(task1_code,NULL,85,NULL,TASK1_PRIORITY,&welcome);
	
	/*start Scheduler */
	vTaskStartScheduler();

	return 0;
    
}
void vApplicationIdleHook(void)
{
	
}

/*Task1 Code */
 void task1_code(void*pvParameters)
{
    static int x = 0;
    static int flag = 0;
	static int counter = 0;
    portTickType count = xTaskGetTickCount();
    while(1)
	{   
    vTaskDelayUntil(&count,50);
    if (x >= 0 && x<= 10 && flag == 0)
    {
	    lcd_clrScreen();
	    lcd_gotoxy(0,x);
	    lcd_dispString("welcome");
	    x++;
	    if (x>10) flag = 1;
    }
    else if (flag == 1)
    {
	    lcd_clrScreen();
	    lcd_gotoxy(0,x);
	    lcd_dispString("welcome");
	    x--;
	    if (x == 0) 
		{
			flag = 0;
			counter++;
		if (counter == 3)
		{	
		lcd_clrScreen();
		xTaskCreate(task2_code,NULL,85,NULL,TASK2_PRIORITY,&task2);
		vTaskDelete(welcome);
		}
		}
    }
   } 
}

/*Task 2 Code*/
   void task2_code(void*pvParamter)
   {
	  u8 flag = 0;
	  u8 temp = 0;
	   static int counter = 0;
	   static int counter2 = 0; 
	   portTickType coun = xTaskGetTickCount(); 
	while(1)
	{
		vTaskDelayUntil(&coun,25);
		counter++;
		counter2++;
		if (KeyPad_u8GetPressedKey() != NO_KEY_PRESSED)
		{
		temp = KeyPad_u8GetPressedKey();
		flag = 1;
		continue;
		}
		else if(flag == 1) 
		{
		 lcd_clrScreen();
		 op_value = temp;
		 xTaskCreate(Normal_op,NULL,85,NULL,TASK3_PRIORITY,&Normal);	
		 vTaskDelete(task2);
		}
		if (counter2 >= 400) break;  
		
		if (counter == 10 || counter == 20)
		{
			lcd_clrScreen();
			lcd_gotoxy(0,0);
			lcd_dispString("Press any Key to");
			lcd_gotoxy(1,1);
			lcd_dispString("Continue");
		}
		
		 if (counter == 30)
		{
			lcd_clrScreen();
			counter = 0;
		}
		
	}
	xTaskCreate(Blinking,NULL,85,NULL,TASK3_PRIORITY,&led);	
	vTaskDelete(task2);
  }
  
  
  void Normal_op(void*pvParamter)
  {
	  portTickType coun = xTaskGetTickCount();
	  lcd_clrScreen();
	  static char finite = 0;
	  char temp = 0;
	  char i = 0;
	  char j = 0;
	  char data[20] = {0};
	  char data2[20] = {0};
	  unsigned char num = 0;
	  unsigned char index = 0;
	  unsigned char index2 = 0;
	  unsigned char flag = 0;
	  unsigned char counter = 0;
	    
	  while(1)
	  {
		    vTaskDelayUntil(&coun,5);
			if (op_value>=48 && op_value<=57 && flag == 0)
			{
			  finite = 0;
			  data[0] = op_value;
			  counter++;
			  flag = 1;
			}
		if(counter != 1)
		{	
			switch (finite)
			{
			 case 0:
			 if(KeyPad_u8GetPressedKey() != NO_KEY_PRESSED)
			 {
				 temp = KeyPad_u8GetPressedKey();
				 num = temp;
				 flag = 2;
				 continue;
			 }
			 
			 if (num >= 48 && num <= 57 && flag == 2)
			 {
				 index++;
				 data[index] = num;
				 lcd_gotoxy(0,index);
				 lcd_displayChar(data[index]);
				 finite = 0;
				 flag = 1;
			 }
			 else if(((num == 43) || (num == 42) || (num == 45) || (num == 47)) && (flag == 2))
			 {
				 finite = 1;
				 operations[ind_op] = num;
				 ind_op++;
				 flag = 1;
			 }
			 else {
				finite = 0; 
			 }
			 break;
			 
			 case 1:
			 if(KeyPad_u8GetPressedKey() != NO_KEY_PRESSED)
			 {
				 temp = KeyPad_u8GetPressedKey();
				 num = temp;
				 flag = 2;
				 continue;
			 }
			 if (num >= 48 && num <= 57 && flag == 2)
			 {
				 if (index2 == 0)
				 lcd_clrScreen();
				 data2[index2] = num;
				 lcd_gotoxy(0,index2);
				 lcd_displayChar(data2[index2]);
				 finite = 1;
				 index2++;
				 flag = 1;
			 }
			 else if(((num == 43) || (num == 42) || (num == 45) || (num == 47)) && (flag == 2))
			 {
				 finite = 2;
				 operations[ind_op] = num;
				 ind_op++;
				 flag = 1;
			 }
			 break;
			 
			 case 2:
			 if ( i <= index)
			 {
				  num1 = num1*10 + (data[i] - 48);
			      i++;
			 }
			 if (j < index2)
			 {
				  num2 = num2*10 + (data2[j]-48);
				  j++;
			 }
			 if ((j >= index2) && (i > index))
			 {
				 xTaskCreate(display_res,NULL,85,NULL,TASK4_PRIORITY,&display);
				 vTaskDelete(Normal);	
			 }
			  break;
			}
		}
		else 
		 {
		 lcd_gotoxy(0,0);
		 lcd_displayChar(data[0]);
		 counter++;
		 flag = 1;
		 }
	  }
  }
  
  
  void display_res(void*pvParamter)
  {
	   portTickType counter = xTaskGetTickCount();
	   char i = 0;
	   while(1)
	   {
	     vTaskDelayUntil(&counter,50);
		 //lcd_clrScreen();
		/* result = num1 + num2;
		 if (result != 0)
		 {
			res[i++] = (result % 10) + '0';
			result = result/10;
			continue; 
		 }
		 if(result == 0)*/
		 lcd_dispString(res);
	   }
  }
  
 void Blinking(void*pvParamter)
 {
	  PWM0_Init();
	  portTickType count = xTaskGetTickCount();
	  static int state = 0 ,duty = 0 ,counter = 0;
	  while (1)
	  {
		  vTaskDelayUntil(&count,250);
		  switch (state)
		  {
			  case 0:
			  PWM0_Init();
			  if(duty<=100)
			  {
				  PWM0_SetDutyCycle(duty);
				  duty += 10;
			  }
			  else{
				  duty = 0;
				  state=1;
			  }
			  break;
			  case 1:
			  PWM0_Stop();
			  if (counter == 10)
			  {
				  state = 2;
			  }
			  else{
				  counter++;
			  }
			  break;
			  case 2:
			  PWM0_Init();
			  if(duty<=100)
			  {
				  PWM0_SetDutyCycle(100-duty);
				  duty += 10;
			  }
			  else{
				  duty = 0;
				  state=3;
			  }
			  break;
			  case 3:
			  PWM0_Stop();
			  if (counter == 0)
			  {
				  state = 0;
			  }
			  else{
				  counter--;
			  }
			  break;
		  }
	  }
	  
  }