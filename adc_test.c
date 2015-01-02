/*
 *	Testing ADC
 *
 * 	Measuring voltage on variable resistor and displaying on 7-segments indicators
 * 	 
 *
 *	D12-D4 - Segments
 *	A0-A4 - Digits
 *	A5 - 5 Channel of ADC
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "common.h"

#define NUM_DIG 10

// Interrupt when ADC complete conversion
float adc_data = 0;
char adc_res[4] = {0,0,0,0};
float adc_tmp = 0;
ISR(ADC_vect)
{

	adc_data = ADCL;
	adc_data |= (ADCH<<8);

}

// 5ms interval

char j=0;

//unsigned char sec = 0;

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

int main() {
	
	DDRC = 0x0F;
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
	
	/* ADC settings
	
		ADMUX:
			REFS1:0=01 	-- 	Ref source connected to AVCC pin with 
						 	capasitor in AREF pin
			MUX3:0=0101 -- 	5 Channel in ADC
			ADLAR=1		--	Left adjusted result in Data Registers (ADCH & ADCL)

		ADCSRA:
			ADPS2:0=111	--	Source to ADC is CLK/128
			ADIE=1		--	Interrupt enable
			ADIF=1		--	Interrupt flag when conversion complete
			ADATE=1		--	Without triggering source, free mode
			ADEN=1 		--	Enable ADC	
			ADSC=1		--	Start conversion
		PRR:
			PRADC=0 	-- Power Reduciton Off		
	*/
	
	ADMUX   = (5<<MUX0);
	ADMUX  |= (1<<REFS0);
	ADMUX  |= (1<<ADLAR);

	ADCSRA  = (7<<ADPS0);
	ADCSRA |= (1<<ADIE);
	ADCSRA |= (1<<ADATE);
	ADCSRA |= (1<<ADEN);
	ADCSRA |= (1<<ADSC);

	// Allow global interrupt
	sei();

	U16 i = 0;
	while(1)
	{
		PORTB = digits[adc_res[j]].PB;
		PORTD = digits[adc_res[j]].PD;	
		
		


				
	}
}
