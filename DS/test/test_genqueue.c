#include <stdio.h>
#include "../inc/genqueue.h"

/* ========================================================== */
/* Test Helper Functions                      */
/* ========================================================== */

static void PrintTestResult(const char* _testName, int _isPass)
{
    if (_isPass)
    {
        printf("[PASS] %s\n", _testName);
    }
    else
    {
        printf("[FAIL] %s\n", _testName);
    }
}

/* Dummy action for QueueForEach: prints integer, returns 1 to continue */
static int PrintIntAction(void* _element, void* _context)
{
    int* val = (int*)_element;
    (void)_context; /* Suppresses the unused parameter warning */
    
    printf("%d ", *val);
    return 1; 
}

/* ========================================================== */
/* Test Cases                         */
/* ========================================================== */

static void TestQueueCreateAndDestroy(void)
{
    Queue* q = QueueCreate(5);
    PrintTestResult("QueueCreate valid size", q != NULL);
    
    QueueDestroy(&q, NULL);
    PrintTestResult("QueueDestroy sets pointer to NULL", q == NULL);
}

static void TestQueueInsertValid(void)
{
    Queue* q = QueueCreate(5);
    int data = 42;
    QueueResult res = QueueInsert(q, &data);
    
    PrintTestResult("QueueInsert valid item", res == QUEUE_SUCCESS);
    QueueDestroy(&q, NULL);
}

static void TestQueueOverflow(void)
{
    Queue* q = QueueCreate(2);
    int d1 = 1, d2 = 2, d3 = 3;
    QueueResult res;
    
    QueueInsert(q, &d1);
    QueueInsert(q, &d2);
    res = QueueInsert(q, &d3);
    
    PrintTestResult("QueueInsert overflow handled", res == QUEUE_OVERFLOW_ERROR);
    QueueDestroy(&q, NULL);
}

static void TestQueueRemoveValid(void)
{
    Queue* q = QueueCreate(5);
    int dataIn = 100;
    int* dataOut = NULL;
    
    QueueInsert(q, &dataIn);
    QueueRemove(q, (void**)&dataOut);
    
    PrintTestResult("QueueRemove valid item", dataOut != NULL && *dataOut == 100);
    PrintTestResult("QueueIsEmpty after remove", QueueIsEmpty(q) != 0);
    QueueDestroy(&q, NULL);
}

static void TestQueueCircularWrap(void)
{
    Queue* q = QueueCreate(3);
    int d[4] = {1, 2, 3, 4};
    int* out;
    QueueResult res; /* Declaration moved to the top block for ANSI C89 */
    
    /* Fill queue: [1, 2, 3] */
    QueueInsert(q, &d[0]);
    QueueInsert(q, &d[1]);
    QueueInsert(q, &d[2]);
    
    /* Remove one: [_, 2, 3] -> head advances */
    QueueRemove(q, (void**)&out);
    
    /* Insert one: [4, 2, 3] -> tail wraps around to index 0 */
    res = QueueInsert(q, &d[3]); /* Assignment only, no declaration here */
    
    PrintTestResult("Queue Circular Buffer Wrap Around", res == QUEUE_SUCCESS);
    QueueDestroy(&q, NULL);
}

static void TestQueueForEach(void)
{
    Queue* q = QueueCreate(5);
    int d[3] = {10, 20, 30};
    size_t count;
    
    QueueInsert(q, &d[0]);
    QueueInsert(q, &d[1]);
    QueueInsert(q, &d[2]);
    
    printf("    ForEach Output: ");
    count = QueueForEach(q, PrintIntAction, NULL);
    printf("\n");
    
    PrintTestResult("QueueForEach counts correct iterations", count == 3);
    QueueDestroy(&q, NULL);
}

/* ========================================================== */
/* Main                            */
/* ========================================================== */

int main(void)
{
    printf("--- Queue Test Suite ---\n");
    TestQueueCreateAndDestroy();
    TestQueueInsertValid();
    TestQueueOverflow();
    TestQueueRemoveValid();
    TestQueueCircularWrap();
    TestQueueForEach();
    
    return 0;
}