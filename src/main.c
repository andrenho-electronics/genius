#include "queue.h"

#include <stdint.h>

#define SHOW_AGAIN 4

static void
mcu_init()
{
}

static void
display(uint8_t opt)
{
    (void) opt;
}

static uint8_t
wait_for_input()
{
    return 0;
}

static void
enter_error_condition()
{
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
}

// vim:st=4:sts=4:sw=4:expandtab
