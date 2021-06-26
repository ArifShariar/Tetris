
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
#include "lcd.h"

int main(void)

{

	DDRD |= (1 << PIND0);//PORTD pin0 as INPUT

	int UBRR_Value = 25; // 2400 baud rate

	UBRRH = (unsigned char) (UBRR_Value >> 8);

	UBRRL = (unsigned char) UBRR_Value;

	UCSRB = (1 << RXEN) | (1 << TXEN);

	UCSRC = (1 << USBS) | (3 << UCSZ0);

	unsigned char receiveData;

	while (1)

	{

		while (! (UCSRA & (1 << RXC)) );

		receiveData = UDR;

		if (receiveData == 0b11110000)

		{
			DDRD = 0x0F;
			DDRC = 0xD0;
			Lcd4_Init();
			while(1)
			{
				Lcd4_Set_Cursor(1,1);
				Lcd4_Write_String("Connection");
				Lcd4_Set_Cursor(2,1);
				Lcd4_Write_String("Successful.");
			}

			

		}

	}

}
