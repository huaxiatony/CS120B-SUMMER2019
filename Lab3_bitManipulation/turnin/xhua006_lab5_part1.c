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
enum States {Start, INIT, LIGHT, WAIT}state;

void Tick1(){
		if((~PINA & 0x00) == 0x00)
		{
			PORTC = 0x40;
		}
		if ((~PINA & 0x01) == 0x01 || (~PINA & 0x02) == 0x02)
		{
			PORTC = 0x60;
		}
		if ((~PINA & 0x03) == 0x03 || (~PINA & 0x04) == 0x04)
		{
			PORTC = 0x70;
		}
		if ((~PINA & 0x05) == 0x05 || (~PINA & 0x06) == 0x06)
		{
			PORTC = 0x38;
		} 
		if ((~PINA & 0x07) == 0x07 || (~PINA & 0x08) == 0x08 || (~PINA & 0x09) == 0x09)
		{
			PORTC = 0x3C;
		}
		if ((~PINA & 0x0A) == 0x0A || (~PINA & 0x0B) == 0x0B || (~PINA & 0x0C) == 0x0C)
		{
			PORTC = 0x3E;
		}
		if ((~PINA & 0x0D) == 0x0D || (~PINA & 0x0E) == 0x0E || (~PINA & 0x0F) == 0x0F)
		{
			PORTC = 0x3F;
		}	
}

void Tick2(){
	switch(state){//Transitions
		case Start:
		PORTC = 0x00;
		state = INIT;
		break;
		
		case INIT:
		if((~PINA & 0x01) == 0x01)
		{
			state = LIGHT; break;
		}
		else
		{
			state = INIT; break;
		}
		
		case LIGHT:
		state = WAIT;
		break;
		
		case WAIT:
		if((~PINA & 0x01) == 0x01)
		{
			state = WAIT; break;
		}
		else if((~PINA & 0x01) == 0x00)
		{
			state = INIT; break;
		}
		
		default:
		break;
	}
	
	switch(state){ //State actions
		case Start:
		break;
		
		case INIT:
		break;
		
		case LIGHT:
		if(PORTC == 0x00)
		{
			PORTC = 0x01; break;
		}
		else if(PORTC == 0x1F)
		{
			PORTC = 0x00; break;
		}
		else
		{
			PORTC = (PORTC << 1);
			if((PORTC & 0x04) == 0x04)
			{
				PORTC = PORTC | 0x01; break;
			}
			break;
		}
		
		case WAIT:
		break;
		
		default:
		break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	//DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	state = Start;
	//unsigned char  = 0x00;
    /* Insert your solution below */
    if ((~PINA & 0x08) == 0x00){
		while (1) {
		Tick2();
    }
    }
    else{
		while(1){
		Tick1();
	}
	}

    return 1;
}
