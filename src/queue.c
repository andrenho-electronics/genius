#include "queue.h"

#include <stdlib.h>

static int seed = 0;
static volatile uint16_t _queue_size = 1;

void
queue_init()
{
    // TODO - initialize seed
    _queue_size = 1;
}

uint8_t
queue_size()
{
    return _queue_size;
}

uint8_t
queue_item(uint8_t i)
{
    srand(seed);

    uint8_t prev = 0xff;
    uint8_t r;
    for (int j = 0; j < (i+1); ++j) {
try_again:
        r = rand() % 4;
        if (r == prev)
            goto try_again;
        prev = r;
    }
    return r;
}

void
queue_increase()
{
    ++_queue_size;
}

// vim:st=4:sts=4:sw=4:expandtab
