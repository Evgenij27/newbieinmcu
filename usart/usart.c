/*
 *	Testing USART0
 *
 * 	Transmit character to computer
 *
 */

#include <inttypes.h>
#include <util/atomic.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "common.h"

#define ZERO   1
#define FIRST  2
#define SECOND 3
#define THIRD  0

char k = 0;

void send_char(char data);
void print(char *str);

unsigned char get_char(void);
char* get_str(void);
void adc2ind(U16 adc_d, char *);

volatile U16 adc_data = 0;
volatile U16 prev_adc_data = 0;

char disp[4]={0,0,0,0};

char digit_table[10] = {48,49,50,51,52,53,54,55,56,57};

ISR(ADC_vect)
{
	adc_data  = ADCL;
	adc_data |= (ADCH<<8);
	//print("OK!\n\r");
	

//	ATOMIC_BLOCK(ATOMIC_FORCEON)
//	{
	if(adc_data != prev_adc_data)
		{	
		prev_adc_data = adc_data;
		
		adc2ind(adc_data, &disp[0]);

		send_char(digit_table[disp[THIRD]]);
		send_char(46);
		send_char(digit_table[disp[SECOND]]);
		send_char(digit_table[disp[FIRST]]);
		send_char(digit_table[disp[ZERO]]);

		disp[0]=0;
		disp[1]=0;
		disp[2]=0;
		disp[3]=0;
		
		for(k=0; k<5; k++)
			send_char(8);
//		ADCSRA |= (1<<ADSC);
	
		}
//	}
	ADCSRA |= (1<<ADSC);
/*	
	adc2ind(adc_data, &disp[0]);
	send_char(digit_table[disp[THIRD]]);
	send_char(46);
	send_char(digit_table[disp[SECOND]]);
	send_char(digit_table[disp[FIRST]]);
	send_char(digit_table[disp[ZERO]]);
	print("\n\r");
*/	

/*	if(k < 20)
		ADCSRA |= (1<<ADSC);
	else
		ADCSRA |= (0<<ADSC);
*/	
}


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

// ADC Settings

	ADMUX   = (5<<MUX0) | (1<<REFS0);
	ADCSRA  = (7<<ADPS0) | (1<<ADIE) | (1<<ADEN);
	ADCSRA |= (1<<ADSC); 

	sei();

	char* mes=" Hello World!";
	char* pm;
	char i=0;

	pm = &mes[0];
	char* c;
	unsigned char ch;
	while(1)
	{		 			
	//	print(mes);		
		while( (ch = get_char()) != '\n' )
		{
			if(ch >= 'a' && ch <= 'z')
			{
				send_char(ch);
			}
		}
				
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


void adc2ind(U16 adc_d, char *disp)
{
	while(adc_d != 0)
	{
		adc_d = adc_d - 1;
		disp[1] = disp[1] + 5;
		if(disp[1]>9)
		{
			disp[1]=0;
			disp[2]++;
			if(disp[2]>9)
			{
				disp[2]=0;
				disp[3]++;
				if(disp[3]>9)
				{
					disp[3]=0;
					disp[0]++;
					if(disp[3]>0 && disp[2]>0 && disp[1]>0 && disp[0]>5)
					{
						disp[0]=5;
						disp[3]=0;
						disp[2]=0;
						disp[1]=0;
					}
				}
			}
		}
	}
}
