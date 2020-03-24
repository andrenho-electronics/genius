#include "queue.h"

#include <stdlib.h>
#include <avr/eeprom.h>

static uint32_t seed = 0;
static volatile uint16_t _queue_size = 1;

void
queue_init()
{
    eeprom_busy_wait();
    seed = eeprom_read_dword((uint32_t*) 0);
    eeprom_busy_wait();
    eeprom_write_dword((uint32_t*) 0, seed + 1);  // seed to use on the next reboot
    srand(seed);
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
    uint8_t r = 0;
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
