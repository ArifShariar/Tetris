/*
 * MultipleLEDMatrix.c
 *
 * Created: 20-Jun-21 11:47:26 AM
 * Author : User
 */ 

#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#define delay 300

#include <avr/io.h>
#include <util/delay.h>





char i,scan = 0x01;
int main(void)
{

	DDRD = 0X00;
	DDRD |= 1 << PIND1;//pin1 of portD as OUTPUT
	DDRD &= ~(1 << PIND0);//pin0 of portD as INPUT
	PORTD |= 1 << PIND0;
	int UBBRValue = 25;//AS described before setting baud rate
	//Put the upper part of the baud number here (bits 8 to 11)
	UBRRH = (unsigned char) (UBBRValue >> 8);
	//Put the remaining part of the baud number here
	UBRRL = (unsigned char) UBBRValue;
	//Enable the receiver and transmitter
	UCSRB = (1 << RXEN) | (1 << TXEN);
	//Set 2 stop bits and data bit length is 8-bit

	UCSRC = (1 << USBS) | (3 << UCSZ0);
    while (1) 
    {
		
		if (PIND & 0x10)//once button is pressed

		{

			while (! (UCSRA & (1 << UDRE)) );

			{

				UDR = 0b11110000;//once transmitter is ready sent eight bit data

			}

			// Get that data outa here!

			_delay_ms(220);
		
		}
		
		
		
		DDRA = 0XFF; //column, we have to shift column
		DDRB = 0XFF; //matrix select
		DDRC = 0XFF; // row select
		DDRD = 0XFF;
		scan = 0x01;
		
		PORTB = 0b00000001;
		
		PORTC = 0b00110000;
		PORTA = 0b11000000;
		_delay_ms(delay);
		
		PORTC = 0b00110000;
		PORTA = 0b00110000;
		_delay_ms(delay);
		
		
		
		PORTC = 0b00110000;
		PORTA = 0b00001100;
		_delay_ms(delay);
		
		PORTC = 0b00110000;
		PORTA = 0b00000011;
		_delay_ms(delay);
		
		PORTC = 0b00000000;
		PORTA = 0b00000000;
		
		
		PORTB=0b00000010;
		
		
		PORTC = 0b00110000;
		PORTA = 0b11000000;
		_delay_ms(delay);
		
		PORTC = 0b00110000;
		PORTA = 0b00110000;
		_delay_ms(delay);
		
		
		
		PORTC = 0b00110000;
		PORTA = 0b00001100;
		_delay_ms(delay);
		
		PORTC = 0b00110000;
		PORTA = 0b00000011;
		_delay_ms(delay);
		
		PORTC = 0b00000000;
		PORTA = 0b00000000;
		
    }
}

