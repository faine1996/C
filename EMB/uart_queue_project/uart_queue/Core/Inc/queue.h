#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>

typedef struct 
{
    char* data;
    uint16_t size;
    uint16_t head;
    uint16_t tail;
    uint16_t count;
} Queue;

Queue* Queue_create(uint16_t size);
int Queue_enque(Queue* queue, char* data, uint16_t size);
char Queue_getChar(Queue* queue);
uint16_t Queue_size(Queue* queue);

#endif