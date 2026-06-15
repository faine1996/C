#include "queue.h"
#include <stdlib.h>

Queue* Queue_create(uint16_t size)
{
    Queue* q;
    
    if ( 0 == size )
    {
        return NULL;
    }
    
    q = (Queue*)malloc(sizeof(Queue));
    if ( NULL == q )
    {
        return NULL;
    }
    
    q->data = (char*)malloc(size * sizeof(char));
    if ( NULL == q->data )
    {
        free(q);
        return NULL;
    }
    
    q->size = size;
    q->head = 0;
    q->tail = 0;
    q->count = 0;
    
    return q;
}

int Queue_enque(Queue* queue, char* data, uint16_t size)
{
    uint16_t i;
    
    if ( NULL == queue || NULL == data )
    {
        return 0;
    }
    
    /* Crocodile check: Ensure buffer overflow is prevented */
    if ( queue->size < (queue->count + size) )
    {
        return 0;
    }
    
    for ( i = 0; i < size; ++i )
    {
        queue->data[queue->tail] = data[i];
        queue->tail = (queue->tail + 1) % queue->size;
        queue->count++;
    }
    
    return 1;
}

char Queue_getChar(Queue* queue)
{
    char c;
    
    if ( NULL == queue || 0 == queue->count )
    {
        return 0;
    }
    
    c = queue->data[queue->head];
    queue->head = (queue->head + 1) % queue->size;
    queue->count--;
    
    return c;
}

uint16_t Queue_size(Queue* queue)
{
    if ( NULL == queue )
    {
        return 0;
    }
    
    return queue->count;
}