#include <stdlib.h>
#include "../inc/adt_stack.h"

struct Stack
{
    char* data;
    size_t size;
    size_t capacity;
};

/* --- Static Helper Declarations --- */
static int expandCapacity(Stack* _stack);

/* --- Main API Functions --- */

Stack* StackCreate(size_t _initialCapacity)
{
    Stack* stack = NULL;
    
    if (0 == _initialCapacity)
    {
        return NULL;
    }

    stack = (Stack*)malloc(sizeof(Stack));
    
    if (NULL == stack)
    {
        return NULL;
    }

    stack->data = (char*)malloc(_initialCapacity * sizeof(char));
    
    if (NULL == stack->data)
    {
        free(stack);
        return NULL;
    }

    stack->size = 0;
    stack->capacity = _initialCapacity;

    return stack;
}

void StackDestroy(Stack** _stack)
{
    if (NULL != _stack && NULL != *_stack)
    {
        free((*_stack)->data);
        free(*_stack);
        *_stack = NULL;
    }
}

StackResult StackPush(Stack* _stack, int _item)
{
    if (NULL == _stack)
    {
        return STACK_UNINITIALIZED_ERROR;
    }

    if (_stack->size == _stack->capacity)
    {
        if (0 == expandCapacity(_stack))
        {
            return STACK_OVERFLOW_ERROR;
        }
    }

    _stack->data[_stack->size] = _item;
    _stack->size++;
    
    return STACK_SUCCESS;
}

StackResult StackPop(Stack* _stack, int* _item)
{
    if (NULL == _stack || NULL == _item)
    {
        return STACK_UNINITIALIZED_ERROR;
    }

    if (0 == _stack->size)
    {
        return STACK_DATA_NOT_FOUND_ERROR;
    }

    _stack->size--;
    *_item = _stack->data[_stack->size];
    
    return STACK_SUCCESS;
}

size_t StackIsEmpty(const Stack* _stack)
{
    if (NULL == _stack)
    {
        return 1;
    }
    
    return (0 == _stack->size) ? 1 : 0;
}

/* --- Helper Function Definitions --- */

static int expandCapacity(Stack* _stack)
{
    size_t newCapacity = 0;
    char* newData = NULL;

    if (NULL == _stack)
    {
        return 0;
    }

    newCapacity = _stack->capacity * 2;
    newData = (char*)realloc(_stack->data, newCapacity * sizeof(char));
    
    if (NULL == newData)
    {
        return 0;
    }
    
    _stack->data = newData;
    _stack->capacity = newCapacity;
    
    return 1;
}