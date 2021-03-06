/*
 *	D12-D4 - Segments
 *	A0-A4 - Digits
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#define NUM_DIG 10

// 1 ms interval

char j=0;

ISR(TIMER0_COMPA_vect)
{
	PORTC = (1<<j);
	j++;
	if(j>3)
		j=0;
	
	
}

struct digit {
	unsigned char PB;
	unsigned char PD;
} digits[NUM_DIG] = 
{
	0x01, 0x00,
	0xEF, 0xDF,
	0x10, 0x80,
	0x04, 0x80,
	0x0C, 0x40,
	0x04, 0x20,
	0x00, 0x20,
	0x0D, 0x80,
	0x00, 0x00,
	0x04, 0x00
};

// 1 sec interval

char i=0;
ISR(TIMER1_COMPA_vect)
{
	
	//PORTB=digits[i].PB;
	//PORTD=digits[i].PD;
	i++;
	if(i>(NUM_DIG-1))
		i=0;
	
}
int main() {

	DDRC = 0xFF;
	DDRB = 0x1F;
	DDRD = 0xE0;

	// Interrupt on compare: 0x4E
	// Prescaler bits: CS02 and CS00
	// CTC is ON
	
	// CTC Enable
	TCCR0A = (1<<WGM01);
	// Prescaler settings 	
	TCCR0B = (1<<CS02) | (1<<CS00);
   	// Enable interrupt on comapre for Timer0
	TIMSK0 = (1<<OCIE0A);
	// Interrup when Timer value equal with 0x4E
	OCR0A = 0x4E;
	
	// Interrupt on compare: 0x3D09
	// Prescaler bits: CS12 and CS10 
	// CTC is ON
	
	// Enable CTC
	TCCR1A = (1<<WGM12);
	// Prescaler settings 
	TCCR1B = (1<<CS12)|(1<<CS10);
	// Interrupt compare value
	OCR1AH = 0x3D;
	OCR1AL = 0x09;
	// Enable interrupt on compare for Timer1
	TIMSK1 = (1<<OCIE1A);
	
	// Allow interrupt
	sei();

	while(1)
	{

		PORTB = digits[i].PB;
		PORTD = digits[i].PD;		
	}
}
