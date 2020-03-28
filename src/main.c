#include <stdint.h>

#include "input.h"
#include "output.h"
#include "queue.h"
#include "sound.h"

#include <util/delay.h>

int
main()
{
    input_init();
    output_init();
    sound_init();
    sound_play(0);
    for (;;);
    /*
reset:
    queue_init();
    
    while (1) {
        // show lights
show_again:
        for (uint8_t i = 0; i < queue_size(); ++i) {
            output_data(queue_item(i));
            _delay_ms(450);
        }

        // wait for inputs
        for (uint8_t i = 0; i < queue_size(); ++i) {
            uint8_t b = input_wait();
            if (b == SHOW_AGAIN)
                goto show_again;
            uint8_t item = queue_item(i);
            if (b == item) {
                // correct
                output_data(item);
            } else {
                // incorrect
                output_error();
                goto reset;
            }
        }

        // increase queue
        queue_increase();
        _delay_ms(900);
    }
    */
}

// vim:st=4:sts=4:sw=4:expandtab
