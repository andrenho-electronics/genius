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
    DDRD |= _BV(DDD0) | _BV(DDD1) | _BV(DDD2) | _BV(DDD3);
    PORTD |= _BV(PORTD0) | _BV(PORTD1) | _BV(PORTD2) | _BV(PORTD3);
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
    return 0;
}

static void
enter_error_condition()
{
    while (1) {
        PORTD |= _BV(PORTD0) | _BV(PORTD1) | _BV(PORTD2) | _BV(PORTD3);
        _delay_ms(450);
        PORTD &= 0b11110000;
        _delay_ms(450);
    }
}

int
main()
{
    mcu_init();
reset:
    for(;;);
    /*
    queue_init();
    
    while (1) {
        // show lights
show_again:
        for (uint8_t i = 0; i < queue_size(); ++i)
            display(queue_item(i));

        // wait for inputs
        for (uint8_t i = 0; i < queue_size(); ++i) {
            uint8_t b = wait_for_input();
            if (b == SHOW_AGAIN)
                goto show_again;
            if (b != queue_item(i)) {
                enter_error_condition();
                while (wait_for_input() != SHOW_AGAIN);
                goto reset;
            }
        }

        // increase queue
        queue_increase();
    }
    */
}

// vim:st=4:sts=4:sw=4:expandtab
