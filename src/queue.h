#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdint.h>

void queue_init();

uint8_t queue_size();
uint8_t queue_item(uint8_t i);
void    queue_increase();

#endif

// vim:st=4:sts=4:sw=4:expandtab
