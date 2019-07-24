/*
 * GccApplication1.c
 *
 * Created: 7/6/2019 6:36:32 PM
 * Author : Xia Hua
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include "bit.h"
#include "timer.h"
#include "scheduler.h"
#include "keypad.h"
#include "io.h"
#include "io.c"
//#include <stdio.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
/*
Scheduler module
*/ 
static task task1, task2,task3, task4,task5;
task *tasks[] = {&task1, &task2, &task3, &task4, &task5};

/*
Seven Segment Values
*/
#define SegOne   0x01
#define SegTwo   0x02
#define SegThree 0x04
#define SegFour  0x08
char seg_code[]={63,6,91,79,102,109,125,7,127,111};// 0 - 9

/*
KeyPad Values
*/
unsigned int keyPressed = '\0';
unsigned char playCrusor = 0;
unsigned char mapTravel = 0;
char upperMap[4] = {2,8,20,25};
char lowerMap[2] = {10,15};
char upperMapReset[4] = {2,8,20,25};
char lowerMapReset[2] = {10,15};

void lcdMatrix()
{
	PORTA = 0b11010011;
	PORTB = 0b00101001;
}

void sevenSeg()
{
	 for(int l = 0;l >= 0;l-- )
	 {
		 for(int k = 0; k >= 0; k--)
		 {
			 for(int j = 4; j >= 0; j--)
			 {
				 for(int i = 5;i >= 0;i--)
				 {
					 for(int q = 0;q<=125;q++)
					 {
						 PORTB=seg_code[l];
						 PORTD=~SegOne;
						 _delay_ms(1);
						 PORTB=seg_code[k];
						 PORTD=~SegTwo;
						 _delay_ms(1);
						 PORTB=seg_code[j];
						 PORTD=~SegThree;
						 _delay_ms(1);
						 PORTB=seg_code[i];
						 PORTD=~SegFour;
						 _delay_ms(1);
					 }
				 }
			 }
		 }
	 }
}

void customLCD()
{
	char i;
	//Custom character dictionary
	unsigned char Character1[8] = { 4, 10, 5, 31, 20, 6, 10, 18 };  //the character icon
	unsigned char Character2[8] = { 31, 31, 31, 31, 31, 31, 31, 31 }; //the block icon
	unsigned char Character3[8] = { 0x04, 0x0E, 0x0E, 0x0E, 0x1F, 0x00, 0x04, 0x00 }; //undefined
	unsigned char Character4[8] = { 0x01, 0x03, 0x07, 0x1F, 0x1F, 0x07, 0x03, 0x01 }; //undefined
	unsigned char Character5[8] = { 4, 4, 14, 21, 31, 14, 10, 21}; //the boss icon
	unsigned char Character6[8] = { 31, 21, 21, 31, 17, 21, 31,0 }; // the monster icon 
	unsigned char Character7[8] = { 0, 0, 10, 0, 4, 17, 14, 0 }; // the smile icon
	unsigned char Character8[8] = { 0, 10, 31, 31, 14, 4, 0, 0 }; // the heart icon

	LCD_Init();
	
	LCD_Custom_Char(0, Character1);  /* Build Character1 at position 0 */
	LCD_Custom_Char(1, Character2);  /* Build Character2 at position 1 */
	LCD_Custom_Char(2, Character2);  /* Build Character3 at position 2 */ //changed to undefined 
	LCD_Custom_Char(3, Character2);  /* Build Character4 at position 3 */ //changed to undefined
	LCD_Custom_Char(4, Character5);  /* Build Character5 at position 4 */
	LCD_Custom_Char(5, Character6);  /* Build Character6 at position 5 */
	LCD_Custom_Char(6, Character7);  /* Build Character6 at position 6 */
	LCD_Custom_Char(7, Character8);  /* Build Character6 at position 7 */

	LCD_Command(0x80);		/*cursor at home position */
	LCD_String("Side Scroller!!!");
	LCD_Command(0xc0);
	LCD_Char(0);
	LCD_Char(1);
	LCD_Char(4);
	LCD_Char(4);

	LCD_String("Press*Start");	
}

void crusorAt(unsigned char row, unsigned char col){
	if(row == 1){
		if(col <= 16){
			LCD_Cursor(col+1);
		}
	}
	else if(row == 2){
		if(col <= 16){
			LCD_Cursor(col+17);
		}
	}
}

enum gameStage {start, wait, gamePress,gameRelease,win,lose}; //This is the main game function
int gameTick(int state){
	unsigned char i = 0;
	int previousState = state;
	switch(state)
	{
		case start:
		state = wait;
		break;
		case wait:
		if (keyPressed == '*'){
			state = gamePress;
		}
		else{
			state = wait;
		}
		break;
		case gamePress:
		if (keyPressed != '*'){
			state = gameRelease;
		}
		else{
			state = gamePress;
		}
		break;
		case gameRelease:
		if (keyPressed == '*'){
			state = wait;
		}
		else{
			if(mapTravel >= 30){
				state = win;
			}
			else{
				for(i=0 ; i<4 ;i++){
					if(playCrusor == 1 && upperMap[i]+1==1){
						state = lose;
					}
				}
				for (i =0; i<2;i++){
					if(playCrusor == 17 && lowerMap[i]+17==17){
						state = lose;
					}
				}
			}
		}
		break;
		case win:
		if(keyPressed == '*'){
			state = wait;
		}
		else{
			state =win;
		}
		break;
		case lose:
		if(keyPressed == '*'){
			state = wait;
		}
		else{
			state =lose;
		}
		break;
		default:
		break;
	}
	
	switch(state){
		case start:
		break;
		case wait:
		playCrusor = 17;
		memcpy (upperMap, upperMapReset, sizeof (upperMap));
		memcpy (lowerMap, lowerMapReset, sizeof (lowerMap));
		if(previousState != wait){
			LCD_DisplayString(1, "START  Press * to play!");
		}
		break;
		case gamePress:
		break;
		case gameRelease:
		break;
		case win:
		LCD_DisplayString(1, "GAME WIN        your score is:");
		LCD_Cursor(31);
		LCD_WriteData(mapScrollCount/10+'0');
		LCD_Cursor(32);
		LCD_WriteData(mapScrollCount%10+'0');
		break;
		case lose:
		LCD_DisplayString(1, "GAME OVER       your score is:");
		LCD_Cursor(31);
		LCD_WriteData(mapScrollCount/10+'0');
		LCD_Cursor(32);
		LCD_WriteData(mapScrollCount%10+'0');
		break;
		default:
		break;
	}
	return state;
}

enum mapStage {mapwait,mapmove}; //This controls character moves.
int mapTick(int state){
	unsigned char i = 0;
	switch(state){
		case mapwait:
			if(task1.state == gameRelease){
				state = mapmove;
			}
			else{
				state = mapwait;
			}
		break;
			case mapmove:
			if(task1.state == gameRelease){
				state = mapmove;
			}
			else{
				state = mapwait;
			}
			break;
		default:
			break;
	}		
	switch(state){
		case mapwait:
			mapTravel = 0;
			break;
		case mapmove:
			LCD_ClearScreen();
			for(i = 0; i < 4; i++){
				upperMap[i]--;
			}
			for(i = 0; i < 2; i++){
				lowerMap[i]--;
			}
			mapTravel++;
			
			for(i=0;i<4;i++){
				if(upperMap[i] <= 16){
					cursorAt(1,upperMap[i]);
					LCD_WriteData('A');
				}
			}
			for(i=0; i <2;i++){
				if(lowerMap[i]<=16){
					cursorAt(2.lowerMap[i]);
					LCD_WriteData('A');
				}
			}
			break;
		default:
			break;
	}
	return state;
}

enum keypadInput{keypadwait,keypadpress,keypadrelease}; // This detects the keypad pressing
int keypadTick(int state){
	keyPressed = GetKeypadKey();
	switch(state){
		case keypadwait:
		if(keyPressed == '\0'){
			state = keypadwait;
		}
		else{
			state = keypadpress;
		}
		break;
		case keypadpress:
		state = keypadrelease;
		break;
		case keypadrelease:
		if(keyPressed = '\0'){
			state = keypadwait;
		}
		else{
			state = keypadrelease;
		}
		break;
		default:
		break;
	}
	
	switch(state){
		case keypadwait:
		break;
		case keypadpress:
		if(keyPressed == 'A'){
			if(playCrusor == 17){
				playCrusor = 1;
			}
		}
		else if(keyPressed == 'B'){
			if(playCrusor == 1){
				playCrusor = 17;
			}
		}
		break;
		case keypadrelease:
		break;
		break;
		default:
		break;
	}
	return state;
}

enum finaStage{finalwait, finalplayer};// This controls character showing on the screen.
int finalTick(int state){
	switch(state){
		case finalwait:
			if(task1.state == gameRelease){
				state=finalplayer; 
			}
			else{
				state=finalwait;
			}
			break;
		case finalplayer:
			if(task1.state == gameRelease){
				state=finalplayer;
			}
			else{
				state=finalwait;
			}
			break;
		default:
			break;
	}

	switch (state)
	{
		case finalwait:
			break;
		case finalplayer:
			LCD_Cursor(playerPos);
			LCD_WriteCommand(0x0C);
			LCD_WriteData('G');
			break;
		default:
			break;
	}
	return state;
}

int main() {
	/* Configure the ports as output */
	DDRA = 0xF0; PORTA = 0x0F; // KeyPad Entry
	DDRB = 0xFF; PORTB = 0x00; // 7 Segment Matrix
	DDRC = 0xFF; PORTC = 0x00; // LCD Screen Control 
	DDRD = 0xFF; PORTD = 0x00; // LCD Control Signal for PD5,6,7. Also, 7 Segment Matrix digit control. Maybe Data Output?	
	
	customLCD(); // This is the Welcome screen.
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	/*
	Task doing state
	*/
	task1.state =  start;
	task1.period = 10;
	task1.elapsedTime = task1.period;
	task1.TickFct = &gameTick;
	
	task2.state = mapWait;
	task2.period =100;
	task2.elapsedTime = task2.period;
	task2.TickFct = &mapTick;
	
	task3.state = playerWait;
	task3.period =10;
	task3.elapsedTime = task3.period;
	task3.TickFct = &keypadTick;
	
	task4.state = printPlayer;
	task4.period =10;
	task4.elapsedTime = task4.period;
	task4.TickFct = &playerPrintTick;

	/*
	task5.state = lightWait;
	task5.period =100;
	task5.elapsedTime = task5.period;
	task5.TickFct = &lightTick;
	*/
	
	/*
	GCD function 
	*/
	unsigned long GCD = tasks[0]->period;
	for(int i = 1;i <numTasks; i++){
		GCD = findGCD(GCD,tasks[i]->period);
	}
	TimerSet(GCD);
	TimerOn();
	
	unsigned short i;
	
	while (1)
	{
		sevenSeg();
		//lcdMatrix();	
		/*
		Something Common
		*/
		for(i = 0; i < numTasks; i++){
			if(tasks[i]->elapsedTime == tasks[i]->period)
			{
				tasks[i]->state= tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			else tasks[i]->elapsedTime += GCD;
		}
			while(!TimerFlag);
			TimerFlag = 0;
	}
	return 1;
}
