#include "sound.h"

#include <avr/interrupt.h>
#include <avr/io.h>

volatile int current_wave = 0;

void sound_init()
{
    // speaker
    DDRD |= (1 << PD4);
}

void sound_play(int note)
{
/*
// TIMER 1 for interrupt frequency 1760.5633802816901 Hz:
cli(); // stop interrupts
TCCR1A = 0; // set entire TCCR1A register to 0
TCCR1B = 0; // same for TCCR1B
TCNT1  = 0; // initialize counter value to 0
// set compare match register for 1760.5633802816901 Hz increments
OCR1A = 212; // = 750000 / (1 * 1760.5633802816901) - 1 (must be <65536)
// turn on CTC mode
TCCR1B |= (1 << WGM12);
// Set CS12, CS11 and CS10 bits for 1 prescaler
TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
// enable timer compare interrupt
TIMSK |= (1 << OCIE1A);
sei(); // allow interrupts
*/
		// TIMER 0 for interrupt frequency 884.433962264151 Hz:
		cli(); // stop interrupts
		TCCR0A = 0; // set entire TCCR0A register to 0
		TCCR0B = 0; // same for TCCR0B
		TCNT0  = 0; // initialize counter value to 0
		// set compare match register for 884.433962264151 Hz increments
		OCR0A = 206; // = 750000 / (8 * 884.433962264151) - 1 (must be <256)
		// turn on CTC mode
		TCCR0A |= (1 << WGM01);
		// Set CS02, CS01 and CS00 bits for 8 prescaler
		TCCR0B |= (0 << CS02) | (1 << CS01) | (0 << CS00);
		// enable timer compare interrupt
		TIMSK |= (1 << OCIE0A);
		sei(); // allow interrupts
}

ISR(TIMER0_COMPA_vect)
{
    current_wave = !current_wave;
    if (current_wave)
        PORTD |= _BV(PORTD4);
    else
        PORTD &= ~_BV(PORTD4);
}

// vim:st=4:sts=4:sw=4:expandtab
