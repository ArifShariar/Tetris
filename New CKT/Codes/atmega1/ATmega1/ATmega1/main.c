/*
 * ATmega1.c
 *
 * Created: 06-Jul-21 9:01:47 AM
 * Author : User
 */ 



#ifndef F_CPU
#define F_CPU 1000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#define delay 300



unsigned char activateBlockA = 0b00010000;
unsigned char activateBlockB = 0b00000001;
int main(void)
{
    DDRA = 0xFF; // column
	DDRC = 0xFF; // row

    while (1) 
    {
		PORTA = 0b11010000;
		PORTC = 0b11100111;
		
		
    }
}

