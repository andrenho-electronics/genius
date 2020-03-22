#include "queue.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include <stdint.h>

#define SHOW_AGAIN 4

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

static void
display(uint8_t opt)
{
    PORTD |= _BV(PORTD0) | _BV(PORTD1) | _BV(PORTD2) | _BV(PORTD3);
    PORTD &= ~(1 << opt);
    _delay_ms(450);
    PORTD |= _BV(PORTD0) | _BV(PORTD1) | _BV(PORTD2) | _BV(PORTD3);
}

static uint8_t
wait_for_input()
{
    int ret = -1;
    while (1) {
        if (!(PINB & _BV(PB0)))
            ret = 0;
        if (!(PINB & _BV(PB1)))
            ret = 1;
        if (!(PINB & _BV(PB2)))
            ret = 2;
        if (!(PINB & _BV(PB3)))
            ret = 3;
        if (!(PINB & _BV(PB4)))
            ret = SHOW_AGAIN;
        if (ret != -1) {
            _delay_ms(100);
            return ret;
        }
    }
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

int
main()
{
    mcu_init();
reset:
    queue_init();
    
    while (1) {
        // show lights
show_again:
        for (uint8_t i = 0; i < queue_size(); ++i)
            display(queue_item(i));

        // wait for inputs
        for (uint8_t i = 0; i < queue_size(); ++i) {
            uint8_t b = wait_for_input();
            uint8_t item = queue_item(i);
            if (b == SHOW_AGAIN)
                goto show_again;
            if (b == item) {
                // correct
                display(item);   // TODO - allow button press while displaying
            } else {
                // incorrect
                enter_error_condition();
                while (wait_for_input() != SHOW_AGAIN);
                goto reset;
            }
        }

        // increase queue
        queue_increase();
    }
}

// vim:st=4:sts=4:sw=4:expandtab
