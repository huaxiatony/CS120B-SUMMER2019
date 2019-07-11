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

void set_PWM(double frequency) {
	// Keeps track of the currently set frequency
	// Will only update the registers when the frequency
	// changes, plays music uninterrupted.
	static double current_frequency;
	if (frequency != current_frequency) {

		if (!frequency) TCCR3B &= 0x08; //stops timer/counter
		else TCCR3B |= 0x03; // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) OCR3A = 0xFFFF;
		
		// prevents OCR3A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) OCR3A = 0x0000;
		
		// set OCR3A based on desired frequency
		else OCR3A = (short)(8000000 / (128 * frequency)) - 1;

		TCNT3 = 0; // resets counter
		current_frequency = frequency;
	}
}
void PWM_on() {
	TCCR3A = (1 << COM3A0);
	// COM3A0: Toggle PB6 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	// WGM32: When counter (TCNT3) matches OCR3A, reset counter
	// CS31 & CS30: Set a prescaler of 64
	set_PWM(0);
}
void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

double tone[9] = {2610.63, 2930.66, 3290.63, 3490.23, 3920.00, 4400.00, 4930.88, 5230.25,0};
unsigned char counter = 9;
unsigned char onFlag = 0;
//enum music{Start,INIT, PLAY, WAIT} state;
enum ThreeLEDsSM{BIT_ZERO, BIT_ONE, BIT_TWO, BIT_ZERO_RE, BIT_ONE_RE, BIT_TWO_RE} state_one;
unsigned char threeLED = 0x00;
enum BlinkingLEDSM{BIT_THREE_ON, BIT_THREE_ON_TWO, BIT_THREE_ON_THREE, BIT_THREE_OFF, BIT_THREE_OFF_TWO, BIT_THREE_OFF_THREE} state_two;
unsigned char blinkingLED = 0x00;
enum CombineLEDsSM{start,init} state;

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

void Tick(){
	switch(state)
	{
		case start:
			state = init;
			break;
		case init:
			state = init;
			PORTB = (threeLED | blinkingLED); 
			break;
	}	
}

int main(void)
{
	//DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	//PWM_on();
	TimerSet(40);
	TimerOn();
	state_one = BIT_ZERO;
	state_two = BIT_THREE_ON;
	state = start;
	
	while(1)
	{
		Tick_ONE();
		Tick_TWO();
		Tick();
		while (!TimerFlag);
        TimerFlag = 0;	
	}
	return 1;
}
