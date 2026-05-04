#include <stdlib.h>
#include "../inc/genheap.h"

/* Assumed Vector API (adjust prefixes if your genvec.h differs):
 * size_t VectorSize(const Vector* _vector);
 * int VectorGet(const Vector* _vector, size_t _index, void** _pValue);
 * int VectorSet(Vector* _vector, size_t _index, void* _value);
 * int VectorAppend(Vector* _vector, void* _item);
 * int VectorRemove(Vector* _vector, void** _pValue); 
 */

#define HEAP_MAGIC_NUMBER 0xDEADBEEF

struct Heap
{
    Vector* m_vector;
    Comparator m_comp;
    size_t m_magicNumber;
};

/* ========================================================================= */
/*                       Helper Function Declarations                        */
/* ========================================================================= */

static void siftUp(Heap* _heap, size_t _index);
static void siftDown(Heap* _heap, size_t _index);
static void swapElements(Vector* _vector, size_t _index1, size_t _index2);
static int isHeapValid(const Heap* _heap);

/* ========================================================================= */
/*                              Main Functions                               */
/* ========================================================================= */

Heap* HeapBuild(Vector* _vector, Comparator _pfComp)
{
    Heap* heap;
    int i;
    size_t size;

    if (NULL == _vector || NULL == _pfComp)
    {
        return NULL;
    }

    heap = (Heap*)malloc(sizeof(Heap));
    if (NULL == heap)
    {
        return NULL;
    }

    heap->m_vector = _vector;
    heap->m_comp = _pfComp;
    heap->m_magicNumber = HEAP_MAGIC_NUMBER;

    size = VectorSize(_vector);
    
    if (0 == size)
    {
        return heap;
    }

    /* 
     * Start from the last parent node and sift down.
     * int is used instead of size_t to safely allow i >= 0 loop termination.
     */
    for (i = (int)(size / 2) - 1; i >= 0; --i)
    {
        siftDown(heap, (size_t)i);
    }

    return heap;
}

Vector* HeapDestroy(Heap** _heap)
{
    Vector* vectorToReturn;

    if (NULL == _heap || NULL == *_heap || !isHeapValid(*_heap))
    {
        return NULL;
    }

    vectorToReturn = (*_heap)->m_vector;
    
    (*_heap)->m_magicNumber = 0; /* Invalidate magic number */
    free(*_heap);
    *_heap = NULL;

    return vectorToReturn;
}

HeapResultCode HeapInsert(Heap* _heap, void* _element)
{
    size_t size;
    int appendResult; /* Assuming VectorAppend returns 0 on success or an error code */

    if (!isHeapValid(_heap))
    {
        return HEAP_NOT_INITIALIZED;
    }

    if (NULL == _element)
    {
        /* Depending on design, you might want to allow or reject NULL elements.
         * Assuming standard behavior: reject or ignore if your vector doesn't support it. */
    }

    appendResult = VectorAppend(_heap->m_vector, _element);
    if (0 != appendResult) 
    {
        return HEAP_REALLOCATION_FAILED;
    }

    size = VectorSize(_heap->m_vector);
    
    if (size > 1)
    {
        siftUp(_heap, size - 1);
    }

    return HEAP_SUCCESS;
}

const void* HeapPeek(const Heap* _heap)
{
    void* element = NULL;

    if (!isHeapValid(_heap) || 0 == VectorSize(_heap->m_vector))
    {
        return NULL;
    }

    VectorGet(_heap->m_vector, 0, &element);
    return element;
}

void* HeapExtract(Heap* _heap)
{
    void* extractedElement = NULL;
    size_t size;

    if (!isHeapValid(_heap) || 0 == VectorSize(_heap->m_vector))
    {
        return NULL;
    }

    size = VectorSize(_heap->m_vector);
    VectorGet(_heap->m_vector, 0, &extractedElement);

    if (1 == size)
    {
        VectorRemove(_heap->m_vector, &extractedElement);
        return extractedElement;
    }

    /* Swap root with last element */
    swapElements(_heap->m_vector, 0, size - 1);
    
    /* Remove last element (which is the old root) */
    VectorRemove(_heap->m_vector, &extractedElement);

    /* Sift the new root down to restore heap property */
    siftDown(_heap, 0);

    return extractedElement;
}

size_t HeapSize(const Heap* _heap)
{
    if (!isHeapValid(_heap))
    {
        return 0;
    }

    return VectorSize(_heap->m_vector);
}

size_t HeapForEach(const Heap* _heap, ActionFunction _act, void* _context)
{
    size_t i;
    size_t size;
    size_t count = 0;
    void* element = NULL;

    if (!isHeapValid(_heap) || NULL == _act)
    {
        return 0;
    }

    size = VectorSize(_heap->m_vector);

    for (i = 0; i < size; ++i)
    {
        VectorGet(_heap->m_vector, i, &element);
        ++count;
        
        if (0 == _act(element, _context))
        {
            break;
        }
    }

    return count;
}


/* ========================================================================= */
/*                       Helper Function Definitions                         */
/* ========================================================================= */

static void siftDown(Heap* _heap, size_t _index)
{
    size_t size;
    size_t leftChildIdx;
    size_t rightChildIdx;
    size_t targetIdx;
    void* leftVal;
    void* rightVal;
    void* targetVal;

    size = VectorSize(_heap->m_vector);

    while (1)
    {
        leftChildIdx = 2 * _index + 1;
        rightChildIdx = 2 * _index + 2;
        targetIdx = _index;

        VectorGet(_heap->m_vector, targetIdx, &targetVal);

        /* Check left child */
        if (leftChildIdx < size)
        {
            VectorGet(_heap->m_vector, leftChildIdx, &leftVal);
            if (_heap->m_comp(leftVal, targetVal) > 0)
            {
                targetIdx = leftChildIdx;
                targetVal = leftVal;
            }
        }

        /* Check right child */
        if (rightChildIdx < size)
        {
            VectorGet(_heap->m_vector, rightChildIdx, &rightVal);
            if (_heap->m_comp(rightVal, targetVal) > 0)
            {
                targetIdx = rightChildIdx;
            }
        }

        /* If neither child is greater, heap property is restored */
        if (targetIdx == _index)
        {
            break;
        }

        swapElements(_heap->m_vector, _index, targetIdx);
        _index = targetIdx;
    }
}

static void siftUp(Heap* _heap, size_t _index)
{
    size_t parentIdx;
    void* parentVal;
    void* currentVal;

    while (_index > 0)
    {
        parentIdx = (_index - 1) / 2;
        
        VectorGet(_heap->m_vector, parentIdx, &parentVal);
        VectorGet(_heap->m_vector, _index, &currentVal);

        if (_heap->m_comp(currentVal, parentVal) > 0)
        {
            swapElements(_heap->m_vector, _index, parentIdx);
            _index = parentIdx;
        }
        else
        {
            break;
        }
    }
}

static void swapElements(Vector* _vector, size_t _index1, size_t _index2)
{
    void* val1 = NULL;
    void* val2 = NULL;

    VectorGet(_vector, _index1, &val1);
    VectorGet(_vector, _index2, &val2);

    VectorSet(_vector, _index1, val2);
    VectorSet(_vector, _index2, val1);
}

static int isHeapValid(const Heap* _heap)
{
    return (NULL != _heap && HEAP_MAGIC_NUMBER == _heap->m_magicNumber);
}