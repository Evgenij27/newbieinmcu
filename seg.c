#include <avr/io.h>
#include <avr/interrupt.h>

#define NUM_DIG 10


char j=0;
ISR(TIMER0_COMPA_vect)
{
	unsigned char sreg = SREG;
	cli();
	PORTC = (1<<j);
	j++;
	if(j>3)
		j=0;
	SREG = sreg;
	sei();
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

char i=0;
ISR(TIMER1_COMPA_vect)
{
	cli();
	unsigned char sreg = SREG;
	//PORTB=digits[i].PB;
	//PORTD=digits[i].PD;
	i++;
	if(i>(NUM_DIG-1))
		i=0;
	SREG = sreg;
	sei();
}
int main() {

	DDRC = 0xFF;
	DDRB = 0x1F;
	DDRD = 0xE0;
	TCCR0A = (1<<WGM01);	
	TCCR0B = (1<<CS02) | (1<<CS00); 
	TIMSK0 = (1<<OCIE0A);
	OCR0A = 0x4E;
	
	TCCR1A = (1<<WGM12);
	TCCR1B = (1<<CS12)|(1<<CS10);
	OCR1AH = 0x3D;
	OCR1AL = 0x09;
	TIMSK1 = (1<<OCIE1A);
	sei();

	while(1)
	{
		PORTB = digits[i].PB;
		PORTD = digits[i].PD;		
	}
}
