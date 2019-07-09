/*	Author: xhua006
 *  	Partner(s) Name: Zhangzheng Tong
 *	Lab Section: 
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


//#include <avr/io.h>
//#include <avr/interrupt.h>
#include "io.c"
#include "io.h"
// TimerISR() sets this to 1. C programmer should clear to 0.
volatile unsigned char TimerFlag = 0;


//Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1 ms ticks

void TimerOn()
{
	// AVR timer/counter controller register TCCR1
	// bit3 = 0; CTC mode (clear timer on compare)
	// bit2bit1bit0 = 011: prescaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 / 64 = 125,000 ticks/s
	// THUS, TCNT! register will count at 125,000 ticks/s
	TCCR1B = 0x0B;
	
	//AVR output compare register OCR1A.
	// Timer interrupt will be generated when TCNT! == OCR1A
	// We want a 1 ms tick. 0.001S * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// ! ms has passed. Thus, we compare to 125.
	OCR1A = 125;
	// AVR timer interrupt mask register
	// bit1: OCIE1A -- enables compare match interrupt
	TIMSK1 = 0x02;
	//Initialize avr counter
	TCNT1=0;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds
	_avr_timer_cntcurr = _avr_timer_M;
	//Enable global interrupts: 0x80: 1000000
	SREG |= 0x80;
}
void TimerOff()
{
	// bit3bit1bit0=000: timer off
	TCCR1B = 0x00;
}
void TimerISR()
{
	TimerFlag = 1;
}
// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect)
{
	// CPU automatically calls when TCNT1 == OCR1
	// (every 1 ms per TimerOn settings)
	// Count down to 0 rather than up to TOP (results in a more efficient comparison)
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0)
	{
		// Call the ISR that the user uses
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M)
{
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
enum States {Start, LED1, LED2, LED3, CHECK, WAIT,RESET}state;
unsigned char counter = 0;
unsigned char msg[7] = "Winner!";
void Tick(){
	switch(state){ 
		case Start:
		{
			counter = 5;
			LCD_Cursor(1);
			LCD_WriteData('5');
			state = LED1;
			break;
		}
		
		case LED1:
		if((~PINA & 0x01) == 0x01)
		{
			state = CHECK; break;
		}
		else
		{
			state = LED2; break;
		}
		
		case LED2:
		if((~PINA & 0x01) == 0x01)
		{
			state = CHECK; break;
		}
		else
		{
			state = LED3; break;
		}
		
		case LED3:
		if((~PINA & 0x01) == 0x01)
		{
			state = CHECK; break;
		}
		else
		{
			state = LED1; break;
		}
		
		case CHECK:
		state = WAIT;
		break;
		
		case WAIT:
		if((~PINA & 0x01) == 0x01)
		{
			state = WAIT; break;
		}
		else
		{
			state = RESET; break;
		}
		
		case RESET:
		if((~PINA & 0x01) == 0x01)
		{
			if(counter >= 9)
			{
				counter = 5;
				LCD_ClearScreen();	
				LCD_Cursor(1);
				LCD_WriteData('5');
				}
				state = LED1;
				break;	
			}
			else
			{
				state = RESTART; break;
			}
		
		default:
		break;
	}
	
	
	
	switch(state){
		case Start:
		{
		}
		break;
		
		case LED1:
		PORTB = 0x01;
		break;
		
		case LED2:
		PORTB = 0x02;
		break;
		
		case LED3:
		PORTB = 0x04;
		break;
		
		case CHECK:
		if(PORTB == 0x02)
			{
				counter = counter +1;
				if(counter >= 9)
				{
					LCD_DisplayString(1, msg); 
					break;
				}
				else
				{
					LCD_Cursor(1);
					LCD_WriteData(counter + '0'); 
					break;
				}
			}
			else
			{
				counter = counter -1;
				if(counter <= 0)
				{
					counter = 0;
				}
				LCD_Cursor(1);
				LCD_WriteData(counter + '0'); 
				break;
			}
		
		case WAIT:
		break;
		
		case RESET:
		break;
		
		default: 
		break;
	}
}

int main(void)
{
	DDRA = 0x00;PORTA = 0xFF;
	DDRB = 0xFF;PORTB = 0x05;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	LCD_init();
	TimerSet(30);
	TimerOn();
	state = Start;
	while(1) {
		Tick();
		while (!TimerFlag);
		TimerFlag = 0;
		// Note: For the above a better style would use a synchSM with TickSM()
		// This example just illustrates the use of the ISR and flag
	}
}
