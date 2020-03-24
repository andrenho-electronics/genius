#include "queue.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include <stdint.h>

#define SHOW_AGAIN 4

//
// initialization
//

static void
mcu_init()
{
    // displays (D0 ~ D3) sink
    DDRD |= _BV(PD0) | _BV(PD1) | _BV(PD2) | _BV(PD3);
    PORTD |= _BV(PORTD0) | _BV(PORTD1) | _BV(PORTD2) | _BV(PORTD3);

    // input pins
    DDRB |= _BV(PB0);  // colors
    DDRB |= _BV(PB1);  // ..
    DDRB |= _BV(PB2);  // ..
    DDRB |= _BV(PB3);  // ..
    DDRB |= _BV(PB4);  // show again
    PORTB |= _BV(PORTB0) | _BV(PORTB1) | _BV(PORTB2) | _BV(PORTB3) | _BV(PORTB4);  // activate pullups
}

// 
// input
//

static int
check_input()
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
            _delay_ms(100);
            while (PINB & _BV(pins[i].pin));  // wait until button is let go
            return pins[i].number;
        }
    }
    return -1;
}

static uint8_t
wait_for_input()
{
    while (1) {
        int ret = check_input();
        if (ret != -1)
            return ret;
    }
}

// 
// output
//

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

static void
output(int opt)
{
    PORTD |= _BV(PORTD0) | _BV(PORTD1) | _BV(PORTD2) | _BV(PORTD3);  // turn leds off
    if (opt >= 0) {
        PORTD &= ~(1 << opt);  // turn single led on
        start_timer();
    }
}

// when interrupt is called, turn off light
ISR(TIMER1_COMPA_vect)
{
    output(-1);
}

static void
enter_error_condition()
{
    while (1) {
        PORTD &= 0b11110000;
        _delay_ms(450);
        PORTD |= _BV(PORTD0) | _BV(PORTD1) | _BV(PORTD2) | _BV(PORTD3);
        _delay_ms(450);
    }
}

// 
// main loop
//

int
main()
{
    mcu_init();
reset:
    queue_init();
    
    while (1) {
        // show lights
        for (uint8_t i = 0; i < queue_size(); ++i) {
            output(queue_item(i));
            _delay_ms(450);
        }

        // wait for inputs
        for (uint8_t i = 0; i < queue_size(); ++i) {
            uint8_t b = wait_for_input();
            uint8_t item = queue_item(i);
            if (b == SHOW_AGAIN)
                continue;
            if (b == item) {
                // correct
                output(item);
            } else {
                // incorrect
                enter_error_condition();
                while (wait_for_input() != SHOW_AGAIN);
                goto reset;
            }
        }

        // increase queue
        queue_increase();
        _delay_ms(900);
    }
}

// vim:st=4:sts=4:sw=4:expandtab
