
/*
 * LCD.c
 *
 * Created: 20-Jul-21 9:52:37 AM
 * Author : User
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"
int i = 0;
int state = 0;

int isStarted = 0;
int isOngoing = 0;


ISR(INT0_vect){
	i++;
}

ISR(INT1_vect){
	
	if(isStarted==0 && isOngoing==0){
		isStarted = 1;
		isOngoing = 1;
		PORTB = 0b00000010;
		_delay_ms(50);
		PORTB = 0b00000000;
	}
	
	
}


int main(void)
{
	DDRD = 0b11110000;
	DDRC = 0xFF;
	DDRB = 0x00;
	
	MCUCR = MCUCR | 0b00000010 | 0b00001000;
	GICR = (1<<INT0)|(1<<INT1);
	sei();
	
	Lcd4_Init();
	while(1)
	{
		
		
		if(isStarted==0){
			Lcd4_Set_Cursor(1,1);
			Lcd4_Write_String("Press START!");
			Lcd4_Clear();
		}
		else if(isStarted==1 && isOngoing==1){
			char score[10];
			sprintf(score,"%d",i);
			Lcd4_Set_Cursor(1,1);
			Lcd4_Write_String("SCORE: ");
			Lcd4_Write_String(score);
		}
		
	}
}
