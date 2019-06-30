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
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
	unsigned char counter;
    /* Insert your solution below */
    while (1) {
		unsigned char result = 0x00;
		for(counter = 0; counter <8 ; counter++){
			result = result + ((PINA & (0x01 << counter)) >> counter); // left shift finder and restore it while found 1 add it to result
			result = result + ((PINB & (0x01 << counter)) >> counter); // left shift finder and restore it while found 1 add it to result
		}
		PORTC = result;
    }
    return 1;
}
