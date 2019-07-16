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
#include "scheduler.h"
#include "io.c"
#include "io.h"
//#include "keypad.h"
#include "timer.h"
#include "bit.h"

//#include "lcd_8bit_task.h"
//#include "queue.h"
//#include "seven_seg.h"
//#include "stack.h"
//#include "usart.h"

// Task scheduler structure
/*typedef struct _task {
	signed char state;
	unsigned long int period;
	unsigned long in elapsedTime;
	int (*TickFct) (int);
} task;*/

//-----------------------------------------------------------------------------------
unsigned char led0_output = 0x00;
unsigned char led1_output = 0x00;
unsigned char pause = 0;
enum pauseButtonSM_States {pauseButton_wait, pauseButton_press, pauseButton_release};
//Monitors button connected to PA0.
//When button is pressed, shared variable "pause" is toggled.
int pauseButtonSMTick(int state){
	unsigned char press = ~PINA & 0x01;
	switch(state){
		case pauseButton_wait:
			state = (press == 0x01)? pauseButton_press: pauseButton_wait; break;
		case pauseButton_press:
			state = pauseButton_release; break;
		case pauseButton_release:
			state = (press == 0x00)? pauseButton_wait: pauseButton_press; break;
		default:
			state = pauseButton_wait;break;
	}
	switch(state){
		case pauseButton_wait:break;
		case pauseButton_press:
			pause = (pause == 0)? 1:0;//toggle pause
			break;
		case pauseButton_release: break;
	}
	return state;
}
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
enum toggleLED0_States { toggleLED0_wait, toggleLED0_blink};

int toggleLED0SMTick(int state){
		switch(state){
			case toggleLED0_wait:state = (!pause)? toggleLED0_blink:toggleLED0_wait;break;
			case toggleLED0_blink: state = (pause)? toggleLED0_wait:toggleLED0_blink;break;
			default: state = toggleLED0_wait; break;
		}
		switch(state){
			case toggleLED0_wait:break;
			case toggleLED0_blink:
				led0_output=(led0_output == 0x00)? 0x01: 0x00;
				break;
		}
		return state;
}
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
enum toggleLED1_States {toggleLED1_wait, toggleLED1_blink};

int toggleLED1SMTick(int state){
		switch(state){
			case toggleLED1_wait: state = (!pause)? toggleLED1_blink: toggleLED1_wait; break;
			case toggleLED1_blink: state = (pause)? toggleLED1_wait:toggleLED1_blink; break;
			default: state = toggleLED1_wait; break;
		}
		switch(state){
			case toggleLED1_wait:break;
			case toggleLED1_blink:
				led1_output = (led1_output == 0x00)? 0x01: 0x00;
				break;
		}
		return state;
}
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
enum display_States {display_display};

int displaySMTick(int state){
		unsigned char output;
		switch(state){
			case display_display: 
					state = display_display;	
				break;
			default: state= display_display;break;
		}
		switch(state){
			case display_display:
				output = led0_output | led1_output << 1;
				break;
		}
		PORTB = output;
		return state;
}

//-----------------------------------------------------------------------------------

int main(void) {
    /* Insert DDR and PORT initializations */
	unsigned char x;
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	static task task1, task2, task3, task4;
    task *tasks[] = {&task1, &task2, &task3, &task4};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
    //DDRC = 0xF0; PORTC = 0x0F;
    /* Insert your solution below */
    
    //Task 1 (pauseButtonToggleSM)
    task1.state = pauseButton_wait;//Task initial state
    task1.period = 1;//Task Period
    task1.elapsedTime = task1.period;//Task current elapsed time.
    task1.TickFct = &pauseButtonSMTick;//Function pointer for the tick
    //Task 2 (toggleLED0SM)
    task2.state = toggleLED0_wait;//Task initial state
    task2.period = 20;//Task Period
    task2.elapsedTime = task2.period;//Task current elapsed time.
    task2.TickFct = &toggleLED0SMTick;//Function pointer for the tick
    //Task 3 (toggleLED1SM)
    task3.state = toggleLED1_wait;//Task initial state
    task3.period = 40;//Task Period
    task3.elapsedTime = task3.period;//Task current elapsed time.
    task3.TickFct = &toggleLED1SMTick;//Function pointer for the tick
    //Task 4 (displaySM)
    task4.state = display_display;//Task initial state
    task4.period = 10;//Task Period
    task4.elapsedTime = task4.period;//Task current elapsed time.
    task4.TickFct = &displaySMTick;//Function pointer for the tick
    //Set the timer and turn it on
	unsigned long int GCD;
    TimerSet(1);
    TimerOn();
    unsigned short i;//Scheduler for-loop iterator
    
    while (1) {
		for(i = 0; i < numTasks; i++){//Scheduler code
			if(tasks[i]->elapsedTime == tasks[i]->period){//Task is ready to tick
				tasks[i]->state= tasks[i]->TickFct(tasks[i]->state);//set next state
				tasks[i]->elapsedTime = 0;//Reset the elapsed time for next tick;
			}
			tasks[i]->elapsedTime += GCD; 
			GCD = findGCD(GCD, tasks[i]->period);
		}
	while(!TimerFlag);
	TimerFlag = 0;
	}
}


