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
#include "scheduler.h"
#include "bit.h"
#include "io.c"
#include "io.h"
#include "keypad.h"
//#include "lcd_8bit_task.h"
#include "queue.h"
//#include "seven_seg.h"
//#include "stack.h"
#include "timer.h"
//#include "usart.h"
    static task task1, task2;
    task *tasks[] = {&task1, &task2};
unsigned char display_string[1] = " ";

enum pauseButtonSM_States {pauseButton_wait, pauseButton_press, pauseButton_release};
unsigned char press = '\0';
int tick(int state){
	press = GetKeypadKey();
	switch(state){
		case pauseButton_wait:
		state = (press == '\0')? pauseButton_wait: pauseButton_press; break;
		case pauseButton_press:
		state = pauseButton_release; break;
		case pauseButton_release:
		state = (press == '\0')? pauseButton_wait: pauseButton_release; break;
		default:
		state = pauseButton_wait;break;
	}
	switch(state){
		case pauseButton_wait:
		break;
		case pauseButton_press:
		break;
		case pauseButton_release:
		break;
		default:
		break;
	}
	return state;
}


enum displayStates{start1,display};
int displaySM(int state)
{
	switch(state)
	{
		case start1:
			state = display;
			break;
		case display:
			break;
		default:
			state = start1;
			break;
	}
	switch(state)
	{
		case start1:
			break;
		case display:
			if(task1.state == pauseButton_press)
			{
				LCD_Cursor(1);
				LCD_WriteData(press);
			}
			break;
	}
	return state;
}
int main(void) {
    /* Insert DDR and PORT initializations */
	/*unsigned char x;
	DDRC = 0xFF; PORTC  = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0xF0; PORTA = 0x0F;

	LCD_init();
	//LCD_DisplayString(1,string);
    /* Insert your solution below */
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
    
    //Task 1 (pauseButtonToggleSM)
    
    task1.state =  pauseButton_wait;//Task initial state
    task1.period = 1;//Task Period
    task1.elapsedTime = task1.period;//Task current elapsed time.
    task1.TickFct = &tick;//Function pointer for the tick
    //Set the timer and turn it on
    
    task2.state = start1;//Task initial state
    task2.period =1;//Task Period
    task2.elapsedTime = task2.period;//Task current elapsed time.
    task2.TickFct = &displaySM;//Function pointer for the tick
	unsigned long int GCD = 5;
    TimerSet(10);
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
    return 1;
}
