/*	Author: Xia Hua xhua006
 *  Partner(s) Name: Zhangzheng Tong
 *	Lab Section:
 *	Assignment: Lab #2  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0x00; PORTC = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;
	unsigned char temp = 0x00;
	unsigned char shift = 0x00;
	while(1){
		temp = (PINA + PINB + PINC) > 140;
		temp = temp + (abs(PINA - PINC)>80 << 1);
		shift = (PINA + PINB + PINC)/3;
		PORTD = (shift << 2) + (temp & 0x03);
	}
	return 0;
}