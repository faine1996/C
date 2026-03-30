#include <stdio.h>

#include "../inc/genvector.h"

void TestVectorCreation(void);
void TestVectorAppendAndGet(void);
void TestVectorRemoveAndCapacity(void);

int main(void)
{
    printf("=== Testing Vector Creation ===\n");
    TestVectorCreation();
    
    printf("=== Testing Vector Append and Get ===\n");
    TestVectorAppendAndGet();
    
    printf("=== Testing Vector Remove and Capacity ===\n");
    TestVectorRemoveAndCapacity();
    
    return 0;
}

void TestVectorCreation(void)
{
    Vector *vec = NULL;
    
    vec = VectorCreate(5, 2);
    if (NULL != vec)
    {
        printf("Vector created successfully.\n");
        printf("Initial Capacity: %lu\n", (unsigned long)VectorCapacity(vec));
        printf("Initial Size: %lu\n\n", (unsigned long)VectorSize(vec));
        VectorDestroy(&vec, NULL);
    }
    else
    {
        printf("Vector creation failed.\n\n");
    }
}

void TestVectorAppendAndGet(void)
{
    Vector *vec = NULL;
    int val1 = 10;
    int val2 = 20;
    int val3 = 30;
    int *retrievedVal = NULL;
    
    vec = VectorCreate(2, 2);
    
    VectorAppend(vec, &val1);
    VectorAppend(vec, &val2);
    VectorAppend(vec, &val3); 
    
    printf("Size after appends: %lu\n", (unsigned long)VectorSize(vec));
    printf("Capacity after appends: %lu\n", (unsigned long)VectorCapacity(vec));
    
    VectorGet(vec, 1, (void**)&retrievedVal);
    if (NULL != retrievedVal)
    {
        printf("Value at index 1: %d\n\n", *retrievedVal);
    }
    
    VectorDestroy(&vec, NULL);
}

void TestVectorRemoveAndCapacity(void)
{
    Vector *vec = NULL;
    int vals[5] = {1, 2, 3, 4, 5};
    int *removedVal = NULL;
    int i = 0;
    
    vec = VectorCreate(2, 2);
    
    for (i = 0; 5 > i; ++i)
    {
        VectorAppend(vec, &vals[i]);
    }
    
    printf("Capacity before removes: %lu\n", (unsigned long)VectorCapacity(vec));
    
    VectorRemove(vec, (void**)&removedVal);
    VectorRemove(vec, (void**)&removedVal);
    VectorRemove(vec, (void**)&removedVal);
    
    printf("Capacity after 3 removes: %lu\n", (unsigned long)VectorCapacity(vec));
    printf("Last removed value: %d\n\n", *removedVal);
    
    VectorDestroy(&vec, NULL);
}