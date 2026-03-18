#include <stdio.h>
#include <stdlib.h>

#include "../inc/dyn_arr.h"

static int paraCheck(const void *ptr);
static int inputCheck(int size, int blockSize);

struct DynamicArray* DynamicArrayCreate(int size, int blockSize)
{
    struct DynamicArray *dyn_arr;
    int status;

    status = inputCheck(size,blockSize);
    if (ERROR == status)
    {
        return NULL;
    }

    dyn_arr = (struct DynamicArray*)malloc(sizeof(struct DynamicArray));
    if (NULL == dyn_arr)
    {
        printf("Unable to allocate memory for struct");
		return NULL;
    }

    dyn_arr->dArraySize = size;
    dyn_arr->dArrayBlockSize = blockSize;
    dyn_arr->NumOfElements = 0;

    dyn_arr->dArray = (int*)malloc(size * sizeof(int));
    if (NULL == dyn_arr->dArray)
    {
        printf("Unable to allocate memory for array within struct");
        free(dyn_arr);
		return NULL;
    }
    return dyn_arr; 
}

void DynamicArrayDestroy(struct DynamicArray *DynamicArrayPtr)
{

    if (DARRAY_NULL_PTR == paraCheck(DynamicArrayPtr)) 
    {
        return;
    }

    free(DynamicArrayPtr->dArray);

    free(DynamicArrayPtr);
}

int DynamicArrayInsert(struct DynamicArray *DynamicArrayPtr, int data)
{
    int resized_flag;
    int new_capacity;
    void *temp_ptr;
    int target_index;

    resized_flag = 0;

    if (DARRAY_NULL_PTR == paraCheck(DynamicArrayPtr)) 
    {
        return DARRAY_NULL_PTR;
    }

    if (DynamicArrayPtr->NumOfElements == DynamicArrayPtr->dArraySize)
    {
        new_capacity = DynamicArrayPtr->dArraySize + DynamicArrayPtr->dArrayBlockSize;

        temp_ptr = (int*)realloc(DynamicArrayPtr->dArray, new_capacity * sizeof(int));

        if (NULL == temp_ptr)
        {
            printf("Reallocation failed, old array still intact cannot insert new element");
            return DARRAY_OVERFLOW;
        }

        DynamicArrayPtr->dArray = temp_ptr;
        DynamicArrayPtr->dArraySize = new_capacity;
        resized_flag = 1;
    }

    target_index = DynamicArrayPtr->NumOfElements;
    DynamicArrayPtr->dArray[target_index] = data;

    DynamicArrayPtr->NumOfElements += 1;

    if (1 == resized_flag)
    {
        return DARRAY_RESIZE;
    }

    return DARRAY_OK;
}

int DynamicArrayDelete(struct DynamicArray *DynaminArrayPtr, int *data)
{
    int last_index;

    if (DARRAY_NULL_PTR == paraCheck(DynaminArrayPtr))
    {
        return DARRAY_NULL_PTR;
    }

    if (DARRAY_NULL_PTR == paraCheck(data))
    {
        return DARRAY_NULL_PTR;
    }

    if (0 == DynaminArrayPtr->NumOfElements)
    {
        return DARRAY_UNDERFLOW;
    }

    last_index = DynaminArrayPtr->NumOfElements - 1;
    *data = DynaminArrayPtr->dArray[last_index];

    DynaminArrayPtr->NumOfElements -= 1;

    return DARRAY_OK;
}

static int paraCheck(const void *ptr) 
{
    if (NULL == ptr) 
    {
        return DARRAY_NULL_PTR;
    }
    return DARRAY_OK;
}

static int inputCheck(int size, int blockSize)
{
    if (0 >= size || 0 >= blockSize) 
    {
        return ERROR;
    }

    return DARRAY_OK;
}


