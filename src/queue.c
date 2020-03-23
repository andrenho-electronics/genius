#include "queue.h"

#include <stdlib.h>

#define MAX_QUEUE 32  // TODO

static volatile uint8_t _queue[MAX_QUEUE / 4];
static volatile uint16_t _queue_size = 1;

static unsigned int
find_srand()
{
    return 4;  // TODO
}

void
queue_init()
{
    srand(find_srand());
    uint8_t prev = 0xff;
    for (int i = 0; i < MAX_QUEUE; ++i) {
        uint8_t nxt;
        do {
            nxt = (rand() % 4);
            int pos = (i % 4) * 2;
            _queue[i / 4] |= nxt << pos;
        } while (nxt == prev);  // don't repeat the same number
        printf("%d ", nxt);
        prev = nxt;
    }
    printf("\n");
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
    int pos = (i % 4) * 2;
    return (_queue[i / 4] >> pos) & 0b11;
}

void
queue_increase()
{
    ++_queue_size;
}

// vim:st=4:sts=4:sw=4:expandtab
