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
//#include "io.c"
//#include "io.h"

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
	OCR1A = 10;
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

double tone[9] = {2610.63, 2930.66, 3290.63, 3490.23, 3920.00, 4400.00, 4930.88, 5230.25,0};
unsigned char counter = 9;
unsigned char onFlag = 0;
enum ThreeLEDsSM{BIT_ZERO, BIT_ONE, BIT_TWO, BIT_ZERO_RE, BIT_ONE_RE, BIT_TWO_RE} state_one;
unsigned char threeLED = 0x00;

enum BlinkingLEDSM{BIT_THREE_ON, BIT_THREE_ON_TWO, BIT_THREE_ON_THREE, BIT_THREE_OFF, BIT_THREE_OFF_TWO, BIT_THREE_OFF_THREE} state_two;
unsigned char blinkingLED = 0x00;

enum CombineLEDsSM{start,init} state;

enum DummySpeaker{new,in,wait,reset} sound;
unsigned char playit =0x00;
void Tick_ONE(){
	switch(state_one)
	{
		case BIT_ZERO:
		{
			state_one = BIT_ONE;
			break;
		}
		case BIT_ONE:
		{
			state_one = BIT_TWO;
			break;
		}
		case BIT_TWO:
		{
			state_one = BIT_ZERO_RE;
			break;
		}
		
		case BIT_ZERO_RE:
		{
			state_one = BIT_ONE_RE;
			break;
		}
		case BIT_ONE_RE:
		{
			state_one = BIT_TWO_RE;
			break;
		}
		case BIT_TWO_RE:
		{
			state_one = BIT_ZERO;
			break;
		}
		
		default:
			break;
	}
	switch(state_one)
	{
		case BIT_ZERO:
		{
			threeLED = 0x01;
			break;
		}
		case BIT_ONE:
		{
			threeLED = 0x02;
			break;
		}
		case BIT_TWO:
		{
			threeLED = 0x04;
			break;
		}
		
		case BIT_ZERO_RE:
		{
			threeLED = 0x01;
			break;
		}
		case BIT_ONE_RE:
		{
			threeLED = 0x02;
			break;
		}
		case BIT_TWO_RE:
		{
			threeLED = 0x04;
			break;
		}
		default:
			break;
	}
}

void Tick_TWO(){
	switch(state_two)
	{
		case BIT_THREE_ON:
		{
			state_two = BIT_THREE_ON_TWO;
			break;
		}
		case BIT_THREE_ON_TWO:
		{
			state_two = BIT_THREE_ON_THREE;
			break;
		}
		case BIT_THREE_ON_THREE:
		{
			state_two = BIT_THREE_OFF;
			break;
		}
		case BIT_THREE_OFF:
		{
			state_two = BIT_THREE_OFF_TWO;
			break;
		}
		case BIT_THREE_OFF_TWO:
		{
			state_two = BIT_THREE_OFF_THREE;
			break;
		}
		case BIT_THREE_OFF_THREE:
		{
			state_two = BIT_THREE_ON;
			break;
		}
		default:
			break;
	}
	
	switch(state_two)
	{
		case BIT_THREE_ON:
		{
			blinkingLED = 0x08;
			break;
		}
		case BIT_THREE_ON_TWO:
		{
			blinkingLED = 0x08;
			break;
		}
		case BIT_THREE_ON_THREE:
		{
			blinkingLED = 0x08;
			break;
		}
		case BIT_THREE_OFF:
		{
			blinkingLED = 0x00;
			break;
		}
		case BIT_THREE_OFF_TWO:
		{
			blinkingLED = 0x00;
			break;
		}
		case BIT_THREE_OFF_THREE:
		{
			blinkingLED = 0x00;
			break;
		}
		default:
			break;
	}
}

unsigned long frequent = 1;
unsigned long fcount;
unsigned long repeat = 100;
unsigned long rcount;

void soundPlayer(){
	switch(sound){
		case new:
			sound = in;
			rcount = repeat;
			break;
		case in:
		if ((~PINA & 0x04) == 0x04)
			{
				sound = wait;
				break;
			}
			else 
			{
				sound = in;
				playit = 0x00;
				fcount = frequent;
				break;
			}
		case wait:
			sound = reset;
			playit = 0x10;
			
			
			if((~PINA & 0x04) != 0x04) {
				sound = in;
			} else {
				if (fcount == 0) {
					sound = reset;
					fcount = frequent;
				} else {
					fcount--;
				}
			}
			break;
		case reset:
			playit = 0x00;
			sound = in;
			if((~PINA & 0x04) != 0x04) {
				sound = in;
			} else {
				if (fcount == 0) {
					sound = wait;
					fcount = frequent;
				} else {
					fcount--;
				}
			}
			break;
		default:
			break;
	}
	
		switch(sound){
		case new:
			break;
		case in:
			break;
		case wait:
			break;
		case reset:
			break;
		default:
			break;
	}
}

enum bSM {buttonStart, button1Pressed, button2Pressed, waitforrelease} bstate;

void button(){
	switch(bstate)
	{
		case buttonStart:
			if((~PINA & 0x01) == 0x01)
				bstate = button1Pressed;
			else if((~PINA & 0x02) == 0x02)
				bstate = button2Pressed;
			else bstate = buttonStart;
			break;
		case button1Pressed:
			if ((~PINA & 0x01) == 0x01)
				bstate = waitforrelease;
			else bstate = button1Pressed;
			break;
		case button2Pressed:
			if ((~PINA & 0x02) == 0x02)
				bstate = waitforrelease;
			else bstate = button1Pressed;
			break;
		case waitforrelease:
			if(~PINA == 0)
				bstate = buttonStart;
			break;
		default:
			bstate = buttonStart;
			break;
	}
	switch(bstate)
	{
		case buttonStart:
			break;
		case button1Pressed:
			//if(frequent < 100)
				frequent+=10;
			break;
		case button2Pressed:
			//if(frequent > 0)
				frequent-=10;
			break;
		case waitforrelease:
			break;
		default:
		break;
			/*if ((~PINA & 0x01) ==0x01)
			{
				
				if (rcount == 0) {
					frequent -=50;
					if(frequent == 0){
						frequent = 1;
					}
					sound = new;
				} else
					sound = in;
			}
			else if ((~PINA & 0x02) ==0x02){
				if (rcount == 0) {
					frequent +=50;
					if(frequent > 1000){
						frequent = 1000;
					}
					sound = new;
				} else
					sound = in;
			}*/
			
	}
	
}



void Tick(){
	switch(state)
	{
		case start:
			state = init;
			break;
		case init:
			state = init;
			PORTB = (threeLED | blinkingLED | playit);
			break;
	}	
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	TimerSet(1);
	TimerOn();
	state_one = BIT_ZERO;
	state_two = BIT_THREE_ON;
	state = start;
	unsigned short finalnumber = 500;
	unsigned short finalcounter = finalnumber;
	
	while(1)
	{
		if(finalcounter != 0) {
			finalcounter--;
		} else {
			Tick_ONE();
			Tick_TWO();
			finalcounter = finalnumber;
		}
		soundPlayer();
		button();
		Tick();
		while (!TimerFlag);
        TimerFlag = 0;	
	}
	return 1;
}

