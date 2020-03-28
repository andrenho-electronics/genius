#include "output.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "input.h"

void
output_init()
{
    // displays (D0 ~ D3) sink
    DDRD |= _BV(PD0) | _BV(PD1) | _BV(PD2) | _BV(PD3);
    PORTD |= _BV(PORTD0) | _BV(PORTD1) | _BV(PORTD2) | _BV(PORTD3);
}

static void
start_timer()
{
    cli(); // stop interrupts
    TCCR1A = 0; // set entire TCCR1A register to 0
    TCCR1B = 0; // same for TCCR1B
    TCNT1  = 0; // initialize counter value to 0
    // set compare match register for 2.222248559982933 Hz increments
    OCR1A = 42186; // = 750000 / (8 * 2.222248559982933) - 1 (must be <65536)
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS12, CS11 and CS10 bits for 8 prescaler
    TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
    // enable timer compare interrupt
    TIMSK |= (1 << OCIE1A);
    sei(); // allow interrupts
}

void
output_data(int opt)
{
    PORTD |= _BV(PORTD0) | _BV(PORTD1) | _BV(PORTD2) | _BV(PORTD3);  // turn leds off
    if (opt >= 0) {
        PORTD &= ~(1 << opt);  // turn single led on
        start_timer();
    }
}

void
output_error()
{
    TIMSK &= ~(1 << OCIE0A);  // disable timer
    while (1) {
        PORTD &= 0b11110000;
        for (int i = 0; i < 45; ++i) {
            if (input_check() == SHOW_AGAIN) {
                output_data(-1);
                _delay_ms(450);
                return;
            }
            _delay_ms(10);
        }
        PORTD |= _BV(PORTD0) | _BV(PORTD1) | _BV(PORTD2) | _BV(PORTD3);
        for (int i = 0; i < 45; ++i) {
            if (input_check() == SHOW_AGAIN) {
                output_data(-1);
                _delay_ms(450);
                return;
            }
            _delay_ms(10);
        }
    }
}

// when interrupt is called, turn off light
/*
ISR(TIMER1_COMPA_vect)
{
    output_data(-1);
}
*/

// vim:st=4:sts=4:sw=4:expandtab
