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
	//DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	//unsigned char tempB = 0x00; 
	//unsigned char tempA = 0x00;
	unsigned char cntavail;
	while(1){
		PORTC = ((PINA & 0x01)==0x01) + 
				((PINA & 0x02)==0x02) +
				((PINA & 0x04)==0x04) +
				((PINA & 0x08)==0x08);
	}
	return 0;
}
