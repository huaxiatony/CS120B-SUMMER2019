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
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRD = 0x00; PORTD = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	unsigned short temp = 0x0000;
	while (1)
	{
		temp = (PIND << 1)|PINB & 0x01;
		if(temp >= 70){
			PORTB = 0x02;
		}
		else if(temp >= 5)
		{
			PORTB = 0x04;
		}
		else{
			PORTB = 0x00;
		}
		temp = 0x0000;
	}
	return 1;
}
