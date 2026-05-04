#include <stdio.h>
#include <stdlib.h>
#include "genheap.h"
#include "genvector.h"

/* ========================================================================= */
/*                       Test Function Declarations                          */
/* ========================================================================= */

static void RunAllTests(void);
static void TestHeapBuild_ValidInputs_ReturnsHeap(void);
static void TestHeapInsert_ValidElement_IncreasesSize(void);
static void TestHeapExtract_MaxHeap_ExtractsDescendingOrder(void);

/* Helper functions for generic data testing */
static int MaxComparator(const void* _left, const void* _right);
static int* CreateInt(int _val);
static void DestroyElement(void* _item);

/* ========================================================================= */
/*                                  Main                                     */
/* ========================================================================= */

int main(void)
{
    RunAllTests();
    return 0;
}

/* ========================================================================= */
/*                       Test Function Definitions                           */
/* ========================================================================= */

static void RunAllTests(void)
{
    TestHeapBuild_ValidInputs_ReturnsHeap();
    TestHeapInsert_ValidElement_IncreasesSize();
    TestHeapExtract_MaxHeap_ExtractsDescendingOrder();
    printf("\nAll tested flows completed successfully.\n");
}

static void TestHeapBuild_ValidInputs_ReturnsHeap(void)
{
    Vector* vec = NULL;
    Heap* heap = NULL;

    vec = VectorCreate(10, 5);
    if (NULL == vec)
    {
        printf("TestHeapBuild_ValidInputs_ReturnsHeap: FAIL (VectorCreate returned NULL)\n");
        return;
    }

    VectorAppend(vec, CreateInt(10));
    VectorAppend(vec, CreateInt(20));
    VectorAppend(vec, CreateInt(5));

    heap = HeapBuild(vec, MaxComparator);
    
    if (NULL != heap)
    {
        printf("TestHeapBuild_ValidInputs_ReturnsHeap: PASS\n");
        
        /* Cleanup */
        vec = HeapDestroy(&heap);
        VectorDestroy(&vec, DestroyElement);
    }
    else
    {
        printf("TestHeapBuild_ValidInputs_ReturnsHeap: FAIL (HeapBuild returned NULL)\n");
        VectorDestroy(&vec, DestroyElement);
    }
}

static void TestHeapInsert_ValidElement_IncreasesSize(void)
{
    Vector* vec = NULL;
    Heap* heap = NULL;
    size_t initialSize;
    size_t newSize;

    vec = VectorCreate(10, 5);
    heap = HeapBuild(vec, MaxComparator);
    
    if (NULL == heap)
    {
        printf("TestHeapInsert_ValidElement_IncreasesSize: FAIL (Setup failed)\n");
        return;
    }

    initialSize = HeapSize(heap);
    HeapInsert(heap, CreateInt(42));
    newSize = HeapSize(heap);

    if (newSize == initialSize + 1)
    {
        printf("TestHeapInsert_ValidElement_IncreasesSize: PASS\n");
    }
    else
    {
        printf("TestHeapInsert_ValidElement_IncreasesSize: FAIL (Size did not increase)\n");
    }

    /* Cleanup */
    vec = HeapDestroy(&heap);
    VectorDestroy(&vec, DestroyElement);
}

static void TestHeapExtract_MaxHeap_ExtractsDescendingOrder(void)
{
    Vector* vec = NULL;
    Heap* heap = NULL;
    int* extracted1 = NULL;
    int* extracted2 = NULL;
    int* extracted3 = NULL;

    vec = VectorCreate(10, 5);
    
    /* We expect Max Heap to extract: 30, then 20, then 10 */
    VectorAppend(vec, CreateInt(10));
    VectorAppend(vec, CreateInt(30));
    VectorAppend(vec, CreateInt(20));

    heap = HeapBuild(vec, MaxComparator);

    extracted1 = (int*)HeapExtract(heap);
    extracted2 = (int*)HeapExtract(heap);
    extracted3 = (int*)HeapExtract(heap);

    if (NULL != extracted1 && NULL != extracted2 && NULL != extracted3 &&
        30 == *extracted1 && 20 == *extracted2 && 10 == *extracted3)
    {
        printf("TestHeapExtract_MaxHeap_ExtractsDescendingOrder: PASS\n");
    }
    else
    {
        printf("TestHeapExtract_MaxHeap_ExtractsDescendingOrder: FAIL (Order incorrect)\n");
    }

    /* We must free the extracted elements manually since they are no longer in the vector */
    if (NULL != extracted1) { free(extracted1); }
    if (NULL != extracted2) { free(extracted2); }
    if (NULL != extracted3) { free(extracted3); }

    /* Cleanup the rest */
    vec = HeapDestroy(&heap);
    VectorDestroy(&vec, DestroyElement);
}

/* ========================================================================= */
/*                       Helper Function Definitions                         */
/* ========================================================================= */

static int MaxComparator(const void* _left, const void* _right)
{
    int leftVal;
    int rightVal;

    if (NULL == _left || NULL == _right)
    {
        return 0;
    }

    leftVal = *(int*)_left;
    rightVal = *(int*)_right;

    if (leftVal > rightVal)
    {
        return 1;
    }
    
    if (leftVal < rightVal)
    {
        return -1;
    }
    
    return 0;
}

static int* CreateInt(int _val)
{
    int* ptr = (int*)malloc(sizeof(int));
    if (NULL != ptr)
    {
        *ptr = _val;
    }
    return ptr;
}

static void DestroyElement(void* _item)
{
    if (NULL != _item)
    {
        free(_item);
    }
}