/*
 * MultipleLEDMatrix.c
 *
 * Created: 20-Jun-21 11:47:26 AM
 * Author : User
 */ 

#ifndef F_CPU
#define F_CPU 1000000UL
#endif



#include <avr/io.h>
#include <util/delay.h>


const char column[64]={
	0x18,0x7c,0x7c,0x1c,0xfe,0x38,0xfe,0x7c,
	0x38,0xc6,0xc6,0x3c,0xc0,0x60,0xc6,0xc6,
	0x18,0x06,0x06,0x6c,0xc0,0xc0,0x0c,0xc6,
	0x18,0x1c,0x3c,0xcc,0xfc,0xfc,0x18,0x7c,
	0x18,0x30,0x06,0xfe,0x06,0xc6,0x30,0xc6,
	0x18,0x66,0xc6,0x0c,0xc6,0xc6,0x30,0xc6,
	0x7e,0xfe,0x7c,0x1e,0x7c,0x7c,0x30,0x7c,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

char i,scan = 0x01;
int main(void)
{
	DDRA = 0XFF;
    DDRB = 0XFF;
	DDRC = 0XFF;
	DDRD = 0XFF;
    while (1) 
    {
		scan = 0x01;
		for (i=0;i<=64;i=i+8)
		{
			PORTC = scan;
			
			PORTB = 0x01;
			PORTA = column[i];
			
			PORTB = 0x02;
			PORTA = column[i+1];
			
			PORTB = 0x04;
			PORTA = column[i+2];
			
			PORTB = 0x08;
			PORTA = column[i+3];
			
			
			PORTB = 0x10;
			PORTA = column[i+4];
			
			PORTB = 0x20;
			PORTA = column[i+5];
			
			PORTB = 0x40;
			PORTA = column[i+6];
			
			
			PORTB = 0x80;
			PORTA = column[i+7];
			
			_delay_us(100);
			
			scan = scan<<1;
		}
    }
}
