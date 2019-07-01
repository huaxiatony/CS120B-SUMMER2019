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
enum dumbMachine{INIT,INC_ONE,DEC_ONE,RESET,WAIT} state;
//0x01 PRESSED  0x00 NOT PRESSED
//
//

void Tick(){
	switch(state){
		case INIT:
			PORTC = 0x07;
			if((PINA & 0x03)==0x01){
				state = INC_ONE;
				break;
			}
			else if((PINA & 0x03)==0x02){
				state = DEC_ONE;
				break;
			}
			else if((PINA & 0x03)==0x03){
				state = RESET;
				break;
			}
			else{
				state = INIT;
				break;
			}
		case INC_ONE:
                        if(PORTC >= 0x09){
				PORTC = 0x09;
				state = WAIT;
				break;
			}
                        else{
                                PORTC = PORTC + 0x01; 
                        	state = WAIT;
			        break;
                        }
		case DEC_ONE:
                        if(PORTC <= 0x00){
                                PORTC = 0x00;
				state = WAIT;
                                break;
                        }
                        else{
                                PORTC = PORTC - 0x01;
                                state = WAIT;
				break;
                        }
		case WAIT:
			if((PINA & 0x03) == 0x01){
				state = INC_ONE;
				break;
			}
			else if((PINA & 0x03)==0x02){	
				state = DEC_ONE;
				break;
			}
			else if((PINA & 0x03) == 0x03){
				state = RESET;
				break;
			}
			else{
				state = WAIT;
				break;
			}
                case RESET:
			if((PINA& 0x03) == 0x03){
                                state = RESET;
                                PORTC = 0x07;
                                break;
			}
                        else{
                                state = INIT;
                                break;
                        }
		default:
			break;
	}
}
int main(void){
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;	
	//DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	state = INIT;
	while (1)
	{
		Tick();
	}
	return 1;
}
