#include <stdlib.h>
#include "../inc/genvector.h"

struct Vector {
    void **items;
    size_t originalCapacity;
    size_t capacity;
    size_t size;
    size_t blockSize;
};

static VectorResult VectorGrow(Vector* _vector);
static VectorResult VectorShrink(Vector* _vector);

Vector* VectorCreate(size_t _initialCapacity, size_t _blockSize)
{
    Vector *vec = NULL;
    
    if (0 == _initialCapacity && 0 == _blockSize)
    {
        return NULL;
    }
    
    vec = (Vector*)malloc(sizeof(Vector));
    if (NULL == vec)
    {
        return NULL;
    }
    
    if (0 == _initialCapacity)
    {
        vec->items = NULL;
    }
    else
    {
        vec->items = (void**)malloc(_initialCapacity * sizeof(void*));
        if (NULL == vec->items)
        {
            free(vec);
            return NULL;
        }
    }
    
    vec->originalCapacity = _initialCapacity;
    vec->capacity = _initialCapacity;
    vec->size = 0;
    vec->blockSize = _blockSize;
    
    return vec;
}

void VectorDestroy(Vector** _vector, void (*_elementDestroy)(void* _item))
{
    size_t i = 0;
    
    if (NULL != _vector && NULL != *_vector)
    {
        if (NULL != _elementDestroy)
        {
            for (i = 0; (*_vector)->size > i; ++i)
            {
                _elementDestroy((*_vector)->items[i]);
            }
        }
        
        if (NULL != (*_vector)->items)
        {
            free((*_vector)->items);
        }
        
        free(*_vector);
        *_vector = NULL;
    }
}

VectorResult VectorAppend(Vector* _vector, void* _item)
{
    VectorResult result = VECTOR_SUCCESS;
    
    if (NULL == _vector)
    {
        return VECTOR_UNITIALIZED_ERROR;
    }
    
    if (_vector->size == _vector->capacity)
    {
        result = VectorGrow(_vector);
        if (VECTOR_SUCCESS != result)
        {
            return result;
        }
    }
    
    _vector->items[_vector->size] = _item;
    _vector->size++;
    
    return VECTOR_SUCCESS;
}

VectorResult VectorRemove(Vector* _vector, void** _pValue)
{
    if (NULL == _vector || NULL == _pValue)
    {
        return VECTOR_UNITIALIZED_ERROR;
    }
    
    if (0 == _vector->size)
    {
        return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
    }
    
    *_pValue = _vector->items[_vector->size - 1];
    _vector->size--;
    
    VectorShrink(_vector);
    
    return VECTOR_SUCCESS;
}

VectorResult VectorGet(const Vector* _vector, size_t _index, void** _pValue)
{
    if (NULL == _vector || NULL == _pValue)
    {
        return VECTOR_UNITIALIZED_ERROR;
    }
    
    if (_vector->size <= _index)
    {
        return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
    }
    
    *_pValue = _vector->items[_index];
    
    return VECTOR_SUCCESS;
}

VectorResult VectorSet(Vector* _vector, size_t _index, void* _value)
{
    if (NULL == _vector)
    {
        return VECTOR_UNITIALIZED_ERROR;
    }
    
    if (_vector->size <= _index)
    {
        return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
    }
    
    _vector->items[_index] = _value;
    
    return VECTOR_SUCCESS;
}

size_t VectorSize(const Vector* _vector)
{
    if (NULL == _vector)
    {
        return 0;
    }
    
    return _vector->size;
}

size_t VectorCapacity(const Vector* _vector)
{
    if (NULL == _vector)
    {
        return 0;
    }
    
    return _vector->capacity;
}

size_t VectorForEach(const Vector* _vector, VectorElementAction _action, void* _context)
{
    size_t i = 0;
    
    if (NULL == _vector || NULL == _action)
    {
        return 0;
    }
    
    for (i = 0; _vector->size > i; ++i)
    {
        if (0 == _action(_vector->items[i], i, _context))
        {
            break;
        }
    }
    
    return i;
}

static VectorResult VectorGrow(Vector* _vector)
{
    void **newItems = NULL;
    size_t newCapacity = 0;
    
    if (0 == _vector->blockSize)
    {
        return VECTOR_ALLOCATION_ERROR;
    }
    
    newCapacity = _vector->capacity + _vector->blockSize;
    newItems = (void**)realloc(_vector->items, newCapacity * sizeof(void*));
    
    if (NULL == newItems)
    {
        return VECTOR_ALLOCATION_ERROR;
    }
    
    _vector->items = newItems;
    _vector->capacity = newCapacity;
    
    return VECTOR_SUCCESS;
}

static VectorResult VectorShrink(Vector* _vector)
{
    void **newItems = NULL;
    size_t newCapacity = 0;
    size_t emptySpaces = 0;
    
    if (0 == _vector->blockSize)
    {
        return VECTOR_SUCCESS;
    }
    
    emptySpaces = _vector->capacity - _vector->size;
    
    if ((_vector->blockSize * 2) <= emptySpaces)
    {
        newCapacity = _vector->capacity - _vector->blockSize;
        
        if (_vector->originalCapacity > newCapacity)
        {
            newCapacity = _vector->originalCapacity;
        }
        
        if (newCapacity == _vector->capacity)
        {
            return VECTOR_SUCCESS;
        }
        
        newItems = (void**)realloc(_vector->items, newCapacity * sizeof(void*));
        
        if (NULL != newItems)
        {
            _vector->items = newItems;
            _vector->capacity = newCapacity;
        }
    }
    
    return VECTOR_SUCCESS;
}