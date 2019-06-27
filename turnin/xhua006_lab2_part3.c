/*	Author: Xia Hua xhua006
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
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;

	unsigned char firstPark = 0x00; 
	unsigned char secondPark = 0x00;
	unsigned char thirdPark = 0x00;
	unsigned char fourthPark = 0x00;
	unsigned char cntavail = 0x00;

	while(1){
		firstPark = PINA & 0x01;
		secondPark = PINA & 0x02;
		thirdPark = PINA & 0x04;
		fourthPark = PINA & 0x08;

		cntavail = firstPark + (secondPark >> 1) + (thirdPark >> 2) + (fourthPark >> 3);
		if(spaceAvail == 0x04)
		{
			PORTC = 0x80;
		}
		else
		{
		cntavail = firstPark + secondPark + thirdPark + fourthPark;
		cntavail = ~cntavail ;
		PORTC = cntavail + & 0x0F;
		}
	}
	return 0;
}
