/*
 * LCD.c
 *
 * Created: 20-Jul-21 9:52:37 AM
 * Author : User
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h" //Can be download from the bottom of this article
int i = 0;
int state = 0;
ISR(INT0_vect){
	i++;
}
int main(void)
{
	DDRD = 0b11110000;
	DDRC = 0xFF;
	DDRB = 0x00;
	
	MCUCR = MCUCR | 0b00000010;
	GICR = (1<<INT0);
	sei();
	
	Lcd4_Init();
	while(1)
	{
		
		char score[10];
		sprintf(score,"%d",i);
		Lcd4_Set_Cursor(1,1);
		Lcd4_Write_String("SCORE: ");
		Lcd4_Write_String(score);
		
	}
}
