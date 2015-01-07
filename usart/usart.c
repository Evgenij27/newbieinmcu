/*
 *	Testing USART0
 *
 * 	Transmit character to computer
 *
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void send_char(char data);
void print(char *str);

unsigned char get_char(void);
char* get_str(void);

int main(void) {
	
/*
 * 	DEVICES SETTINGS
 *
 */

	DDRB = 0xFF;;
	
// 	USART0 Init

//	Set BAUD rate 9600

	UBRR0H = 0x00;
	UBRR0L = 0x67;

//	Set frame format

	UCSR0C = (3<<UCSZ00);

//	Enable Transmitter & Receiver

	UCSR0B = (1<<RXEN0) | (1<<TXEN0);

//	UDRE0

	UCSR0A = (1<<UDRE0);

	char* mes=" Hello World!";
	char* pm;

	pm = &mes[0];
	char* c;
	unsigned char ch;
	while(1)
	{		 			
	//	print(mes);
		
		while( (ch = get_char()) != '\n')
		{
			if(ch >= 'a' && ch <= 'z')
				send_char(ch);
		}
		//print(c);	
	}
}
void send_char(char data)
{
	UDR0 = data;
	while( !(UCSR0A & (1<<UDRE0)) ) 
		{;}
}

void print(char *str)
{
	while(*str != '\0')
	{
		send_char(*str++);
	}
}

unsigned char get_char(void)
{
	unsigned char r_data;

	if( UCSR0A & (1<<RXC0) )
		r_data = UDR0;
	return r_data;	
}

char* get_str(void)
{
	char* r_str = 0;
	char* ps = 0;
	char c = 0;

	while( (c = get_char()) != '\n' )
	{
		*r_str++ = c;
	}
	if((c = get_char()) == '\n')
		*r_str = '\0';

	return &r_str[0];
}
