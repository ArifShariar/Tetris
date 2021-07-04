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
#include <stdio.h>
#include <stdlib.h>








void connectWithOtherAtmega(){
	
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
	if (PIND & 0x10){
		while (! (UCSRA & (1 << UDRE)) ){ //;
			UDR = 0b11110000;//once transmitter is ready sent eight bit data
		}
		_delay_ms(220);
	}
}


unsigned char lastRow = 0x00,lastCol = 0x00;
unsigned char squareBlock[2][10] ={
	{0xC0, 0x30, 0x0C, 0x03,0x00,0xC0,0x30,0x0C,0x03,0x00},
	{0x30, 0x30, 0x30, 0x30,0x00,0x30,0x30,0x30,0x30,0x00}
};
unsigned char Board[2][10] ;


int main(void)
{
	
    while (1) 
    {
		//connectWithOtherAtmega();
		
		///below part is for generating tetris blocks for blocks in the screen
		DDRA = 0XFF; //column, we have to shift column
		DDRB = 0XFF; //matrix select
		DDRC = 0XFF; // row select
		DDRD = 0XFF;
		
		PORTB = 0x01;
		int i;
		for(i=0;i<9;i++){
			if(i<4){
				PORTA = squareBlock[0][i];
				PORTC = squareBlock[1][i];
				 _delay_ms(delay);
			}else if(i==4){
				PORTB = 0b00000010;
				PORTA = squareBlock[0][i];
				PORTC = squareBlock[1][i];
			}
			else{
				PORTA = lastRow| squareBlock[0][i];
				PORTC = lastCol |  squareBlock[1][i];
				 _delay_ms(delay);
			}
			if(PINB1==1 && squareBlock[0][i]==lastRow && lastCol == squareBlock[1][i] ) break;
		}
		lastRow =  lastRow | squareBlock[0][i-1];
		lastCol =  lastCol| squareBlock[1][i-1];
		
		PORTA = lastRow;
		PORTC =  lastCol;
		
		
    }
}

