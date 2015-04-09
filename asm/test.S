#include <avr/io.h> 

#define data r16
#define loopone r17
#define looptwo r18
#define loopthr r19
.global main

main:
        ldi     data,0xFF
        out     DDRB,data 
loop:
        ldi     data,0x00
        out     PORTB,data
        rcall  delay 

        ldi     data,0xFF
        out     PORTB,data
        rcall  delay

        rjmp    loop

delay:
        ldi loopone,0x0F
waitlooptwo:
        ldi looptwo,0xFF
waitloopthr:
        ldi loopthr,0xFF
waitloop:
        subi loopthr,0x01
        brne waitloop
        subi looptwo,0x01
        brne waitloopthr
        subi loopone,0x01
        brne waitlooptwo
        ret
