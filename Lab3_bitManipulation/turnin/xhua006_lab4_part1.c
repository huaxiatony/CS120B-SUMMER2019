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
enum dumbMachine{INIT,TURN_OFF,WAIT_ONE,TURN_ON,WAIT_TWO}state;
//0x01 PRESSED  0x00 NOT PRESSED
//
//

void Tick(){
	switch(state){
		case INIT:
			PORTB = 0x01;
			if((PINA & 0x01)==0x01){
				state = TURN_OFF;
				break;
			}
			else{
				state = INIT;
				break;
			}
		case TURN_OFF:
			PORTB = 0x02;
                        if((PINA & 0x01)==0x00){
                                state = WAIT_ONE;
                                break;
                        }
                        else{
                                state = TURN_OFF;
                                break;
                        }
		case WAIT_ONE:
                        if((PINA & 0x01)==0x01){
                                state = TURN_ON;
                                break;
                        }
                        else{
                                state = WAIT_ONE;
                                break;
                        }
		case TURN_ON:
			PORTB = 0x01;
                        if((PINA & 0x01)==0x00){
                                state = WAIT_TWO;
                                break;
                        }
                        else{
                                state = TURN_ON;
                                break;
                        }
		case WAIT_TWO:
                        if((PINA & 0x01)==0x01){
                                state = TURN_OFF;
                                break;
                        }
                        else{
                                state = WAIT_TWO;
                                break;
                        }
		default:
			break;
	}
}
int main(void){
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;	
	DDRB = 0xFF; PORTB = 0x00;
	//DDRC = 0xFF; PORTC = 0x00;
	state = INIT;
	while (1)
	{
		Tick();
	}
	return 1;
}
