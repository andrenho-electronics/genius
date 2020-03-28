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
		// TIMER 0 for interrupt frequency 884.433962264151 Hz:
		cli(); // stop interrupts
		TCCR0A = 0; // set entire TCCR0A register to 0
		TCCR0B = 0; // same for TCCR0B
		TCNT0  = 0; // initialize counter value to 0
		// set compare match register for 884.433962264151 Hz increments
    switch (note) {
        case -1:
            TCCR0B = 0;  // disable sound
            PORTD &= ~_BV(PORTD4);
            break;
        case 0:
            OCR0A = 34;  // e3
            TCCR0B |= (0 << CS02) | (1 << CS01) | (1 << CS00);  // 64 prescaler
            break; 
        case 1:
            OCR0A = 212;  // a3
            TCCR0B |= (0 << CS02) | (1 << CS01) | (0 << CS00); // 8 prescaler
            break; 
        case 2:
            OCR0A = 168;  // c#4
            TCCR0B |= (0 << CS02) | (1 << CS01) | (0 << CS00); // 8 prescaler
            break; 
        case 3:
            OCR0A = 142;  // e4
            TCCR0B |= (0 << CS02) | (1 << CS01) | (0 << CS00); // 8 prescaler
            break; 
        case 4:
            OCR0A = 150;  // e4
            TCCR0B |= (0 << CS02) | (1 << CS01) | (1 << CS00);  // 64 prescaler
            break; 
    }
		// turn on CTC mode
		TCCR0A |= (1 << WGM01);
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
