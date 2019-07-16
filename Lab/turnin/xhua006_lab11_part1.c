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
#include <avr/interrupt.h>
//#include "scheduler.h"
#include "io.c"
#include "io.h"
#include "keypad.h"
#include "timer.h"
#include "bit.h"

//#include "lcd_8bit_task.h"
//#include "queue.h"
//#include "seven_seg.h"
//#include "stack.h"
//#include "usart.h"

unsigned char key;
//--------Find GCD function -------------------------------
unsigned long int findGCD (unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
		b = c;
	}
	return 0;
}
//--------End find GCD function ---------------------------

//--------Task scheduler data structure--------------------
// Struct for Tasks represent a running process in our
// simple real-time operating system.
/*Tasks should have members that include: state, period, a
measurement of elapsed time, and a function pointer.*/
typedef struct _task {
	//Task's current state, period, and the time elapsed
	// since the last tick
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	//Task tick function
	int (*TickFct)(int);
} task;
//--------End Task scheduler data structure-------------

//--------Shared Variables---------------------------------
unsigned char x = 0x00;
unsigned char key = 0x00;
//--------End Shared Variables-----------------------------


enum states{WAIT, KEYPAIR}state;
int Keypad_tick(int state)
{
	switch(state) //transitions
	{
		case WAIT:
		{
			if(x == 0x1F)
			{
				state = WAIT; break;
			}
			else if(x != 0x1F)
			{
				state = KEYPAIR; break;
			}
		}
		case KEYPAIR:
		{
			state = WAIT; break;
		}
		default:
		{
			state = WAIT;
			x = 0; 
			break;
		}
	}
	switch (state) // state actions
	{
		case WAIT:
		{
			x = GetKeypadKey();
			break;
		}
		
		case KEYPAIR:
		{
			key = x;
			x = 0;
			break;
		}
		default:
			break;
	}
	return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	unsigned char x;
	//DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xF0; PORTC = 0x0F;
    /* Insert your solution below */
    while (1) {
		x =  GetKeypadKey();
		switch(x){
			case '\0': PORTB = 0x1F;break;//All 5 LEDs on
			case '1': PORTB = 0x01; break;//hexequivalent
			case '2': PORTB =0x02; break;
			case '3': PORTB =0x03; break;
			case '4': PORTB =0x04; break;
			case '5': PORTB =0x05; break;
			case '6': PORTB =0x06; break;
			case '7': PORTB =0x07; break;
			case '8': PORTB =0x08; break;
			case '9': PORTB =0x09; break;
			case 'A': PORTB =0x0A; break;
			case 'B': PORTB =0x0B; break;
			case 'C': PORTB =0x0C; break;
			case 'D': PORTB =0x0D; break;
			case '*': PORTB =0x0E; break;
			case '0': PORTB =0x00; break;
			case '#': PORTB =0x0F; break;
			default: PORTB = 0x1B; break;
			//Should never occur, Middle LED off.
		}
    }
    return 1;
}

