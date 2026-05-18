#include <stdlib.h>
#include "../inc/genqueue.h"

#define MAGIC_NUMBER 0xDEADBEEF
#define DEAD_MAGIC_NUMBER 0xDEFACED

struct Queue
{
    void** m_items;
    size_t m_size;
    size_t m_head;
    size_t m_tail;
    size_t m_nItems;
    unsigned int m_magicNumber;
};

/* ========================================================== */
/* Helper Functions                      */
/* ========================================================== */

static int IsValidQueue(Queue* _queue)
{
    return (_queue != NULL && _queue->m_magicNumber == MAGIC_NUMBER);
}

static int IsQueueFull(Queue* _queue)
{
    return (_queue->m_nItems == _queue->m_size);
}

static size_t AdvanceIndex(size_t _currentIndex, size_t _maxSize)
{
    return (_currentIndex + 1) % _maxSize;
}

static void DestroyElements(Queue* _queue, DestroyItem _itemDestroy)
{
    size_t i;
    size_t currentIndex = _queue->m_head;
    
    for (i = 0; i < _queue->m_nItems; ++i)
    {
        _itemDestroy(_queue->m_items[currentIndex]);
        currentIndex = AdvanceIndex(currentIndex, _queue->m_size);
    }
}

/* ========================================================== */
/* API Functions                       */
/* ========================================================== */

Queue* QueueCreate(size_t _size)
{
    Queue* newQueue;

    if (_size == 0)
    {
        return NULL;
    }

    newQueue = (Queue*)malloc(sizeof(Queue));
    if (newQueue == NULL)
    {
        return NULL;
    }

    newQueue->m_items = (void**)malloc(_size * sizeof(void*));
    if (newQueue->m_items == NULL)
    {
        free(newQueue);
        return NULL;
    }

    newQueue->m_size = _size;
    newQueue->m_head = 0;
    newQueue->m_tail = 0;
    newQueue->m_nItems = 0;
    newQueue->m_magicNumber = MAGIC_NUMBER;

    return newQueue;
}

void QueueDestroy(Queue** _queue, DestroyItem _itemDestroy)
{
    if (_queue == NULL || !IsValidQueue(*_queue))
    {
        return;
    }

    if (_itemDestroy != NULL && (*_queue)->m_nItems > 0)
    {
        DestroyElements(*_queue, _itemDestroy);
    }

    (*_queue)->m_magicNumber = DEAD_MAGIC_NUMBER;
    free((*_queue)->m_items);
    free(*_queue);
    *_queue = NULL;
}

QueueResult QueueInsert(Queue* _queue, void* _item)
{
    if (!IsValidQueue(_queue))
    {
        return QUEUE_UNINITIALIZED_ERROR;
    }
    
    if (_item == NULL)
    {
        return QUEUE_DATA_UNINITIALIZED_ERROR;
    }
    
    if (IsQueueFull(_queue))
    {
        return QUEUE_OVERFLOW_ERROR;
    }

    _queue->m_items[_queue->m_tail] = _item;
    _queue->m_tail = AdvanceIndex(_queue->m_tail, _queue->m_size);
    _queue->m_nItems++;

    return QUEUE_SUCCESS;
}

QueueResult QueueRemove(Queue* _queue, void** _item)
{
    if (!IsValidQueue(_queue))
    {
        return QUEUE_UNINITIALIZED_ERROR;
    }
    
    if (_item == NULL)
    {
        return QUEUE_DATA_UNINITIALIZED_ERROR;
    }
    
    if (QueueIsEmpty(_queue))
    {
        return QUEUE_DATA_NOT_FOUND_ERROR;
    }

    *_item = _queue->m_items[_queue->m_head];
    _queue->m_head = AdvanceIndex(_queue->m_head, _queue->m_size);
    _queue->m_nItems--;

    return QUEUE_SUCCESS;
}

size_t QueueIsEmpty(Queue* _queue)
{
    if (!IsValidQueue(_queue))
    {
        return 1; /* Treat invalid as empty */
    }
    
    return (_queue->m_nItems == 0);
}

size_t QueueForEach(Queue* _queue, QueueActionFunction _action, void* _context)
{
    size_t i;
    size_t currentIndex;
    size_t actionsPerformed = 0;

    if (!IsValidQueue(_queue) || _action == NULL)
    {
        return 0;
    }

    currentIndex = _queue->m_head;
    
    for (i = 0; i < _queue->m_nItems; ++i)
    {
        actionsPerformed++;
        if (_action(_queue->m_items[currentIndex], _context) == 0)
        {
            break;
        }
        currentIndex = AdvanceIndex(currentIndex, _queue->m_size);
    }

    return actionsPerformed;
}