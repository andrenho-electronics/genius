#include "input.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "output.h"

void
input_init()
{
    DDRB |= _BV(PB0);  // colors
    DDRB |= _BV(PB1);  // ..
    DDRB |= _BV(PB2);  // ..
    DDRB |= _BV(PB3);  // ..
    DDRB |= _BV(PB4);  // show again
    PORTB |= _BV(PORTB0) | _BV(PORTB1) | _BV(PORTB2) | _BV(PORTB3) | _BV(PORTB4);  // activate pullups
}

int
input_check()
{
    struct Pin {
        int number;
        int pin;
    } pins[] = {
        { 0, PB0 },
        { 1, PB1 },
        { 2, PB2 },
        { 3, PB3 },
        { SHOW_AGAIN, PB4 },
    };

    for (unsigned i = 0; i < sizeof pins / sizeof(struct Pin); ++i) {
        if (!(PINB & _BV(pins[i].pin))) {
            _delay_ms(20);
            while (!(PINB & _BV(pins[i].pin)));  // wait until button is let go
            _delay_ms(10);
            return pins[i].number;
        }
    }
    return -1;
}

uint8_t
input_wait()
{
    while (1) {
        int ret = input_check();
        if (ret != -1)
            return ret;
    }
}

// vim:st=4:sts=4:sw=4:expandtab
