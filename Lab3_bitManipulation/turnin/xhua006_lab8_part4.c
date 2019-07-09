/*	Author: xhua006
 *  Partner(s) Name: Zhangzheng Tong
 *	Lab Section: 
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>
//#include <avr/interrupt.h>
//#include "io.c"
//#include "io.h"

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	// in Free Running Mode, a new conversion will trigger
	// whenever the previous conversion completes.
}
unsigned short max = 0x00;
unsigned short x = 0x00;
unsigned short y = 0x00;
int main(void)
{
	DDRA = 0x00;PORTA = 0xFF;
	DDRB = 0xFF;PORTB = 0x00;
	//DDRC = 0xFF; PORTC = 0x00;
	//DDRD = 0xFF; PORTD = 0x00;
	ADC_init();
	while(1){
		x = ADC;
		if(x > max)
		{
			max = x;
			y = (max/8); 
		}
		else if(x <= y)
		{
			PORTB = 0x00;
		}
		else if(x <= (2*y))
		{
			PORTB = 0x01;
		}
		else if(x <= (3*y))
		{
			PORTB = 0x03;
		}
		else if(x <= (4*y))
		{
			PORTB = 0x07;
		}
		else if(x <= (5*y))
		{
			PORTB = 0x0F;
		}
		else if(x <= (6*y))
		{
			PORTB = 0x1F;
		}
		else if(x <= (7*y))
		{
			PORTB = 0x3F;
		}
		else if(x <= (8*y))
		{
			PORTB = 0x7F; 
		}
		else
		{
			PORTB = 0xFF;
		}
	}
}
