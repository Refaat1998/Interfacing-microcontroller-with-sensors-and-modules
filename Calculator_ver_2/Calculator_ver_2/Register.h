/*
 * Register.h
 *
 *  Created on: Aug 30, 2019
 *      Author: 20120
 */

#ifndef REGISTER_H_
#define REGISTER_H_

#define PORTA	(*((volatile unsigned char*)0x3B))
#define PORTB	(*((volatile unsigned char*)0x38))
#define PORTC	(*((volatile unsigned char*)0x35))
#define PORTD	(*((volatile unsigned char*)0x32))

#define DDRA	(*((volatile unsigned char*)0x3A))
#define DDRB	(*((volatile unsigned char*)0x37))
#define DDRC	(*((volatile unsigned char*)0x34))
#define DDRD	(*((volatile unsigned char*)0x31))

#define PINA	(*((volatile unsigned char*)0x39))
#define PINB	(*((volatile unsigned char*)0x36))
#define PINC	(*((volatile unsigned char*)0x33))
#define PIND	(*((volatile unsigned char*)0x30))

#define SFIOR   (*((volatile unsigned char*)0x50))

#define SREG    (*((volatile unsigned char*)0x5F))
#define MCUCR   (*((volatile unsigned char*)0x55))
#define MCUCSR  (*((volatile unsigned char*)0x54))

#define GICR    (*((volatile unsigned char*)0x5B))
#define GIFR    (*((volatile unsigned char*)0x5A))

#define ADMUX   (*((volatile unsigned char*)0x27))
#define ADCSRA  (*((volatile unsigned char*)0x26))
#define ADCH   	(*((volatile unsigned char*)0x25))
#define ADCL    (*((volatile unsigned char*)0x24))


//Timer

#define TCCR0   (*((volatile unsigned char*)0x53))
#define TCNT0  	(*((volatile unsigned char*)0x52))
#define OCR0   	(*((volatile unsigned char*)0x5C))
#define TIMSK   (*((volatile unsigned char*)0x59))
#define TIFR   	(*((volatile unsigned char*)0x58))

#define TCCR2   (*((volatile unsigned char*)0x45))
#define TCNT2 	(*((volatile unsigned char*)0x44))
#define OCR2   	(*((volatile unsigned char*)0x43))

//UART
#define UDR		(*((volatile unsigned char*)0x2C))
#define UCSRA	(*((volatile unsigned char*)0x2B))
#define UCSRB	(*((volatile unsigned char*)0x2A))
#define UCSRC	(*((volatile unsigned char*)0x40))
#define UBRRH	(*((volatile unsigned char*)0x40))
#define UBRRL	(*((volatile unsigned char*)0x29))

#endif /* REGISTER_H_ */
