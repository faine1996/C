#include <stdio.h>
#include <stdlib.h>
#include "../inc/dyn_arr.h"

void testCreateValid(void);
void testCreateInvalidSize(void);
void testInsertNormal(void);
void testInsertResize(void);
void testInsertNullPtr(void);
void testDeleteNormal(void);
void testDeleteUnderflow(void);
void testDeleteNullPtr(void);
void testDeleteNullData(void);
void testDestroyNormal(void);
void testDestroyNullPtr(void);

int main(void)
{
    printf("--- Running Dynamic Array Tests ---\n\n");

    testCreateValid();
    testCreateInvalidSize();
    testInsertNormal();
    testInsertResize();
    testInsertNullPtr();
    testDeleteNormal();
    testDeleteUnderflow();
    testDeleteNullPtr();
    testDeleteNullData();
    testDestroyNormal();
    testDestroyNullPtr();

    printf("\n--- All Tests Complete ---\n");
    return 0;
}

void testCreateValid(void)
{
    struct DynamicArray *arr;

    printf("Test Create 1 (Valid): Size 2, BlockSize 2.\n");
    arr = DynamicArrayCreate(2, 2);
    printf("Expected: Pointer != NULL, Size: 2, Elements: 0\n");
    
    if (NULL != arr)
    {
        printf("Got     : Pointer != NULL, Size: %d, Elements: %d\n\n", arr->dArraySize, arr->NumOfElements);
        DynamicArrayDestroy(arr);
    }
    else
    {
        printf("Got     : NULL\n\n");
    }
}

void testCreateInvalidSize(void)
{
    struct DynamicArray *arr;

    printf("Test Create 2 (Invalid Size): Size 0, BlockSize 2.\n");
    arr = DynamicArrayCreate(0, 2);
    printf("Expected: Pointer == NULL\n");
    
    if (NULL == arr)
    {
        printf("Got     : Pointer == NULL\n\n");
    }
    else
    {
        printf("Got     : Pointer != NULL\n\n");
        DynamicArrayDestroy(arr);
    }
}

void testInsertNormal(void)
{
    struct DynamicArray *arr;
    int status;

    arr = DynamicArrayCreate(2, 2);
    
    printf("Test Insert 1 (Normal): Inserting 10 into empty array.\n");
    status = DynamicArrayInsert(arr, 10);
    printf("Expected Status: %d, Data[0]: 10, Elements: 1\n", DARRAY_OK);
    printf("Got Status     : %d, Data[0]: %d, Elements: %d\n\n", status, arr->dArray[0], arr->NumOfElements);

    DynamicArrayDestroy(arr);
}

void testInsertResize(void)
{
    struct DynamicArray *arr;
    int status;

    arr = DynamicArrayCreate(2, 2);
    DynamicArrayInsert(arr, 10);
    DynamicArrayInsert(arr, 20);

    printf("Test Insert 2 (Resize): Inserting 30 into full array (Capacity 2).\n");
    status = DynamicArrayInsert(arr, 30);
    printf("Expected Status: %d, Capacity: 4, Elements: 3, Data[2]: 30\n", DARRAY_RESIZE);
    printf("Got Status     : %d, Capacity: %d, Elements: %d, Data[2]: %d\n\n", status, arr->dArraySize, arr->NumOfElements, arr->dArray[2]);

    DynamicArrayDestroy(arr);
}

void testInsertNullPtr(void)
{
    int status;

    printf("Test Insert 3 (NULL Pointer): Passing NULL struct.\n");
    status = DynamicArrayInsert(NULL, 10);
    printf("Expected Status: %d\n", DARRAY_NULL_PTR);
    printf("Got Status     : %d\n\n", status);
}

void testDeleteNormal(void)
{
    struct DynamicArray *arr;
    int status;
    int data;

    arr = DynamicArrayCreate(2, 2);
    DynamicArrayInsert(arr, 50);
    data = 0;

    printf("Test Delete 1 (Normal): Array has 1 element (50).\n");
    status = DynamicArrayDelete(arr, &data);
    printf("Expected Status: %d, Data: 50, Elements: 0\n", DARRAY_OK);
    printf("Got Status     : %d, Data: %d, Elements: %d\n\n", status, data, arr->NumOfElements);

    DynamicArrayDestroy(arr);
}

void testDeleteUnderflow(void)
{
    struct DynamicArray *arr;
    int status;
    int data;

    arr = DynamicArrayCreate(2, 2);
    data = 0;

    printf("Test Delete 2 (Underflow): Array is empty.\n");
    status = DynamicArrayDelete(arr, &data);
    printf("Expected Status: %d\n", DARRAY_UNDERFLOW);
    printf("Got Status     : %d\n\n", status);

    DynamicArrayDestroy(arr);
}

void testDeleteNullPtr(void)
{
    int status;
    int data;

    data = 0;

    printf("Test Delete 3 (NULL Struct Pointer): Passing NULL struct.\n");
    status = DynamicArrayDelete(NULL, &data);
    printf("Expected Status: %d\n", DARRAY_NULL_PTR);
    printf("Got Status     : %d\n\n", status);
}

void testDeleteNullData(void)
{
    struct DynamicArray *arr;
    int status;

    arr = DynamicArrayCreate(2, 2);
    DynamicArrayInsert(arr, 10);

    printf("Test Delete 4 (NULL Data Pointer): Passing NULL for output data.\n");
    status = DynamicArrayDelete(arr, NULL);
    printf("Expected Status: %d\n", DARRAY_NULL_PTR);
    printf("Got Status     : %d\n\n", status);

    DynamicArrayDestroy(arr);
}

void testDestroyNormal(void)
{
    struct DynamicArray *arr;

    arr = DynamicArrayCreate(2, 2);
    
    printf("Test Destroy 1 (Normal): Destroying valid array.\n");
    DynamicArrayDestroy(arr);
    printf("Expected: Function completes without segfault.\n");
    printf("Got     : Function completed.\n\n");
}

void testDestroyNullPtr(void)
{
    printf("Test Destroy 2 (NULL Pointer): Passing NULL to Destroy.\n");
    DynamicArrayDestroy(NULL);
    printf("Expected: Function completes without segfault.\n");
    printf("Got     : Function completed.\n\n");
}