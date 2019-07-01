/*	Author: xhua006
 *  	Partner(s) Name: Zhangzheng Tong
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
enum dumbMachine{INIT,LOCK,PAUSE,WAIT,KEY_Y,KEY_POUND} state;
//0x01 PRESSED  0x00 NOT PRESSED
//
//

void Tick(){
	PORTC = 0x00;
	PORTB = 0x02;
	switch(state){
		case INIT:
			PORTC = 0x01;
			if((PINA & 0x04) == 0x04){
				state = KEY_PUND;
				break;
			}
			else if((PINA & 0x80) == 0x80){
				state = LOCK; 
				break;
			}
			else{
				state = INIT;
			}
		case WAIT:
			PORTC = 0x02;
			if((PINA & 0x02) == 0x02){
				state = KEY_Y;
				break;
			}
			else if((PINA & 0x80) == 0x80){
				state = LOCK;
				break;
			}
			else if((PINA & 0x01) == 0x01){
				state = INIT;	
				break;
			}
			else{
				state = WAIT;
				break;
			}
		case KEY_POUND:
			PORTC = 0x03;
			state = WAIT;
			break;
		case KEY_Y:
			PORTC = 0x04;
			PORTB = 0x01;
			state = PAUSE;
			break;
		case PAUSE:
			if((PINA & 0x87) == 0x00){
				state = INIT;
				break;
			}
			else{
				state = PAUSE;	
				break;
			}

			break;
		case LOCK:
			if((PINA & 0x00) == 0x00){
				state = INIT;
				break;
			}
			else{
				state = LOCK;
				break;
			}
		default:
			break;
}


int main(void){
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;	
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	state = INIT;
	while (1)
	{
		Tick();
	}
	return 1;
}
