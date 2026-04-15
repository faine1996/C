#include <stdlib.h>
#include "../inc/adt_stack.h"

#define MAGIC_NUMBER 0xBEEFCAFE
#define DEAD_MAGIC_NUMBER 0xDEADBEEF

struct Stack
{
    int* m_items;
    size_t m_capacity;
    size_t m_nItems;
    unsigned int m_magicNumber;
};

/* ========================================================== */
/* Helper Functions                                           */
/* ========================================================== */

static int IsValidStack(const Stack* _stack)
{
    return (_stack != NULL && _stack->m_magicNumber == MAGIC_NUMBER);
}

static int IsStackFull(const Stack* _stack)
{
    return (_stack->m_nItems == _stack->m_capacity);
}

/* ========================================================== */
/* API Functions                                              */
/* ========================================================== */

Stack* StackCreate(size_t _initialCapacity)
{
    Stack* newStack;

    if (_initialCapacity == 0)
    {
        return NULL;
    }

    newStack = (Stack*)malloc(sizeof(Stack));
    if (newStack == NULL)
    {
        return NULL;
    }

    newStack->m_items = (int*)malloc(_initialCapacity * sizeof(int));
    if (newStack->m_items == NULL)
    {
        free(newStack);
        return NULL;
    }

    newStack->m_capacity = _initialCapacity;
    newStack->m_nItems = 0;
    newStack->m_magicNumber = MAGIC_NUMBER;

    return newStack;
}

void StackDestroy(Stack** _stack)
{
    if (_stack == NULL || !IsValidStack(*_stack))
    {
        return;
    }

    (*_stack)->m_magicNumber = DEAD_MAGIC_NUMBER;
    
    free((*_stack)->m_items);
    free(*_stack);
    *_stack = NULL;
}

StackResult StackPush(Stack* _stack, int _item)
{
    if (!IsValidStack(_stack))
    {
        return STACK_UNINITIALIZED_ERROR;
    }

    if (IsStackFull(_stack))
    {
        return STACK_OVERFLOW_ERROR;
    }

    _stack->m_items[_stack->m_nItems] = _item;
    _stack->m_nItems++;

    return STACK_SUCCESS;
}

StackResult StackPop(Stack* _stack, int* _item)
{
    if (!IsValidStack(_stack) || _item == NULL)
    {
        return STACK_UNINITIALIZED_ERROR;
    }

    if (StackIsEmpty(_stack))
    {
        return STACK_DATA_NOT_FOUND_ERROR;
    }

    _stack->m_nItems--;
    *_item = _stack->m_items[_stack->m_nItems];

    return STACK_SUCCESS;
}

size_t StackIsEmpty(const Stack* _stack)
{
    if (!IsValidStack(_stack))
    {
        return 1; /* Treat invalid as empty */
    }
    
    return (_stack->m_nItems == 0) ? 1 : 0;
}