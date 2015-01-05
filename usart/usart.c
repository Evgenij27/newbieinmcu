/*
 *	Testing USART0
 *
 * 	Transmit character to computer
 *
 */
#include <avr/io.h>
#include <avr/interrupt.h>

void send_char(char data);

int main(void) {
	
/*
 * 	DEVICES SETTINGS
 *
 */
	DDRB = 0xFF;;
	
// 	USART0 Init

//	Set BAUD rate

	UBRR0H = 0x00;
	UBRR0L = 0x67;

//	Set frame format

	UCSR0C = (3<<UCSZ00);

//	Enable Transmitter & Receiver

	UCSR0B = (1<<RXEN0) | (1<<TXEN0);

//	UDRE0

	UCSR0A = (1<<UDRE0);

	char* mes="Hello World!";
	char* pm;
	pm = &mes[0];

	while(1)
	{
		 
		while(*mes != '\0')
		{

			send_char(*mes++);	
		}

		mes = pm;
		
	}	

}


void send_char(char data)
{
	while( !(UCSR0A & (1<<UDRE0)) ) 
		{;}
	UDR0 = data;
}
