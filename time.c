/*
 *	Clock
 *
 *	D12-D4 - Segments
 *	A0-A4 - Digits
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#define NUM_DIG 10

// 5ms interval

char j=0;
unsigned char k = 0;
//unsigned char sec = 0;

ISR(TIMER0_COMPA_vect)
{
	k++;
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

char time[4] = {0,0,0,0};

int main() {

	DDRC = 0xFF;
	DDRB = 0x1F;
	DDRD = 0xE0;

	// Interrupt on compare: 0x4E (78)
	// Prescaler bits: CS02 and CS00
	// CTC is ON
	
	// CTC Enable
	TCCR0A = (1<<WGM01);
	// Prescaler settings 	
	TCCR0B = (1<<CS02) | (1<<CS00);
   	// Enable interrupt on comapre for Timer0
	TIMSK0 = (1<<OCIE0A);
	// Interrup when Timer value equal with 0x4E (78)
	OCR0A = 0x4E;
	
	// Allow global interrupt
	sei();

	char t = 0;
	while(1)
	{
	
		PORTB = digits[time[j]].PB;
		PORTD = digits[time[j]].PD;	

		if(k == 200)
		{
			k=0;	
			time[1]++;
			
			if (time[1] > 9)
			{
				time[1] = 0;
				time[2]++;

				if (time[2] > 5)
				{
					time[2] = 0;
					time[3]++;

					if (time[3] > 9)
					{
						time[3] = 0;
						time[0]++;

						if (time[0] > 2)
						{
							time[0] = 0;		
						}	
					}
				}
			}
		}	
			
	}
}
