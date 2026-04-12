#include <stdlib.h>
#include <assert.h>
#include "../inc/stack.h"

#define MAGIC_NUMBER 0xBEEFCAFE
#define DEAD_MAGIC_NUMBER 0xDEADBEEF

struct Stack
{
    void** m_items;
    size_t m_originalCapacity;
    size_t m_capacity;
    size_t m_blockSize;
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

static StackResult StackGrow(Stack* _stack)
{
    void** newItems;
    size_t newCapacity;

    if (_stack->m_blockSize == 0)
    {
        return STACK_ALLOCATION_ERROR; /* Act as Overflow for fixed-size stack */
    }

    newCapacity = _stack->m_capacity + _stack->m_blockSize;
    newItems = (void**)realloc(_stack->m_items, newCapacity * sizeof(void*));
    
    if (newItems == NULL)
    {
        return STACK_ALLOCATION_ERROR;
    }

    _stack->m_items = newItems;
    _stack->m_capacity = newCapacity;

    return STACK_SUCCESS;
}

static void StackShrink(Stack* _stack)
{
    void** newItems;
    size_t newCapacity;

    if (_stack->m_blockSize == 0)
    {
        return; /* Fixed size stack, do not shrink */
    }

    /* Hysteresis: Shrink only if there are at least 2 full blocks of empty space */
    if ((_stack->m_capacity - _stack->m_nItems) >= (_stack->m_blockSize * 2))
    {
        newCapacity = _stack->m_capacity - _stack->m_blockSize;
        
        /* Never shrink below the user's initial requested capacity */
        if (newCapacity < _stack->m_originalCapacity)
        {
            newCapacity = _stack->m_originalCapacity;
        }

        if (newCapacity < _stack->m_capacity)
        {
            newItems = (void**)realloc(_stack->m_items, newCapacity * sizeof(void*));
            if (newItems != NULL)
            {
                _stack->m_items = newItems;
                _stack->m_capacity = newCapacity;
            }
        }
    }
}

static void DestroyElements(Stack* _stack, void (*_elementDestroy)(void* _item))
{
    size_t i;
    for (i = 0; i < _stack->m_nItems; ++i)
    {
        _elementDestroy(_stack->m_items[i]);
    }
}

/* ========================================================== */
/* API Functions                                              */
/* ========================================================== */

Stack* StackCreate(size_t _initialCapacity, size_t _blockSize)
{
    Stack* newStack;

    if (_initialCapacity == 0 && _blockSize == 0)
    {
        return NULL;
    }

    newStack = (Stack*)malloc(sizeof(Stack));
    if (newStack == NULL)
    {
        return NULL;
    }

    /* If initial capacity is 0, delay array allocation until first push */
    if (_initialCapacity > 0)
    {
        newStack->m_items = (void**)malloc(_initialCapacity * sizeof(void*));
        if (newStack->m_items == NULL)
        {
            free(newStack);
            return NULL;
        }
    }
    else
    {
        newStack->m_items = NULL;
    }

    newStack->m_originalCapacity = _initialCapacity;
    newStack->m_capacity = _initialCapacity;
    newStack->m_blockSize = _blockSize;
    newStack->m_nItems = 0;
    newStack->m_magicNumber = MAGIC_NUMBER;

    return newStack;
}

void StackDestroy(Stack** _stack, void (*_elementDestroy)(void* _item))
{
    if (_stack == NULL || !IsValidStack(*_stack))
    {
        return;
    }

    if (_elementDestroy != NULL && (*_stack)->m_nItems > 0)
    {
        DestroyElements(*_stack, _elementDestroy);
    }

    (*_stack)->m_magicNumber = DEAD_MAGIC_NUMBER;
    
    if ((*_stack)->m_items != NULL)
    {
        free((*_stack)->m_items);
    }
    
    free(*_stack);
    *_stack = NULL;
}

StackResult StackPush(Stack* _stack, void* _item)
{
    StackResult growResult;

    if (!IsValidStack(_stack))
    {
        return STACK_UNITIALIZED_ERROR;
    }

    if (_stack->m_nItems == _stack->m_capacity)
    {
        growResult = StackGrow(_stack);
        if (growResult != STACK_SUCCESS)
        {
            return growResult;
        }
    }

    _stack->m_items[_stack->m_nItems] = _item;
    _stack->m_nItems++;

    return STACK_SUCCESS;
}

StackResult StackPop(Stack* _stack, void** _pValue)
{
    assert(_pValue != NULL); /* Per the header warning */

    if (!IsValidStack(_stack))
    {
        return STACK_UNITIALIZED_ERROR;
    }

    if (_stack->m_nItems == 0)
    {
        return STACK_IS_EMPTY;
    }

    _stack->m_nItems--;
    *_pValue = _stack->m_items[_stack->m_nItems];

    StackShrink(_stack);

    return STACK_SUCCESS;
}

StackResult StackTop(Stack* _stack, void** _pValue)
{
    assert(_pValue != NULL); /* Per the header warning */

    if (!IsValidStack(_stack))
    {
        return STACK_UNITIALIZED_ERROR;
    }

    if (_stack->m_nItems == 0)
    {
        return STACK_IS_EMPTY;
    }

    *_pValue = _stack->m_items[_stack->m_nItems - 1];

    return STACK_SUCCESS;
}

size_t StackSize(const Stack* _stack)
{
    if (!IsValidStack(_stack))
    {
        return 0;
    }
    return _stack->m_nItems;
}

size_t StackCapacity(const Stack* _stack)
{
    if (!IsValidStack(_stack))
    {
        return 0;
    }
    return _stack->m_capacity;
}

int StackIsEmpty(Stack* _stack)
{
    if (!IsValidStack(_stack))
    {
        return 1; /* Treat invalid as empty */
    }
    return (_stack->m_nItems == 0) ? 1 : 0;
}

size_t StackPrint(const Stack* _stack, StackElementAction _action, void* _context)
{
    size_t i;
    size_t actionsPerformed = 0;

    if (!IsValidStack(_stack) || _action == NULL)
    {
        return 0;
    }

    /* Iterating from top of the stack to the bottom */
    for (i = _stack->m_nItems; i > 0; --i)
    {
        actionsPerformed++;
        if (_action(_stack->m_items[i - 1], i - 1, _context) == 0)
        {
            break;
        }
    }

    return actionsPerformed;
}