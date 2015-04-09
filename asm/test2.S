#define __SFR_IO_OFFSET 0
#include <avr/io.h>

.global main

main:
	ldi r16, (1<<PB5)
	out _SFR_IO_ADDR(DDRB), r16
//	out _SFR_IO_ADDR(PORTB), r16
	ldi r16, (1<<WGM12)|(1<<CS12)
	sts _SFR_IO_ADDR(TCCR1B), r16
	ldi r16, 0x24
	sts _SFR_IO_ADDR(OCR1AH), r16
	ldi r16, 0x9F
	sts _SFR_IO_ADDR(OCR1AL), r16
//	ldi r16, (1<<OCIE1A)
//	sts _SFR_IO_ADDR(TIMSK1), r16
	sei
	rjmp loop

timer:
	ldi r18, 0x00
	out _SFR_IO_ADDR(PORTB), r18
	ret
	
loop: 
	ldi r18, 0xFF
	ldi r19, 0x00
	out _SFR_IO_ADDR(PORTB), r18
	lds r20, _SFR_IO_ADDR(TIFR1)
	sbrc r20, 1
	rcall timer
	out _SFR_IO_ADDR(PORTB), r19
	rjmp loop




