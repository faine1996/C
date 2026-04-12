#include <stdio.h>
#include "../inc/stack.h"

/* ========================================================== */
/* Test Helper Functions                                      */
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

static int PrintIntAction(void* _element, size_t _index, void* _context)
{
    int* val = (int*)_element;
    (void)_index;   /* Suppress unused warning */
    (void)_context; /* Suppress unused warning */
    
    printf("%d ", *val);
    return 1; 
}

/* ========================================================== */
/* Test Cases                                                 */
/* ========================================================== */

static void TestStackCreateAndDestroy(void)
{
    Stack* s = StackCreate(5, 2);
    PrintTestResult("StackCreate valid sizes", s != NULL);
    
    StackDestroy(&s, NULL);
    PrintTestResult("StackDestroy sets pointer to NULL", s == NULL);
}

static void TestStackPushAndTop(void)
{
    Stack* s = StackCreate(5, 0);
    int dataIn = 42;
    int* dataOut = NULL;
    StackResult resPush;
    StackResult resTop;
    
    resPush = StackPush(s, &dataIn);
    resTop = StackTop(s, (void**)&dataOut);
    
    PrintTestResult("StackPush returns SUCCESS", resPush == STACK_SUCCESS);
    PrintTestResult("StackTop fetches correct data", resTop == STACK_SUCCESS && dataOut != NULL && *dataOut == 42);
    
    StackDestroy(&s, NULL);
}

static void TestStackFixedOverflow(void)
{
    Stack* s = StackCreate(2, 0); /* 0 block size = fixed stack */
    int d1 = 1, d2 = 2, d3 = 3;
    StackResult res;
    
    StackPush(s, &d1);
    StackPush(s, &d2);
    res = StackPush(s, &d3); /* Should fail */
    
    PrintTestResult("Fixed Stack Overflow Handled", res == STACK_ALLOCATION_ERROR);
    StackDestroy(&s, NULL);
}

static void TestStackDynamicGrowth(void)
{
    Stack* s = StackCreate(2, 2); /* starts at 2, grows by 2 */
    int d[3] = {10, 20, 30};
    size_t capBefore, capAfter;
    
    capBefore = StackCapacity(s);
    
    StackPush(s, &d[0]);
    StackPush(s, &d[1]);
    StackPush(s, &d[2]); /* Triggers growth */
    
    capAfter = StackCapacity(s);
    
    PrintTestResult("Stack Capacity Grows dynamically", capBefore == 2 && capAfter == 4);
    StackDestroy(&s, NULL);
}

static void TestStackDynamicShrink(void)
{
    Stack* s = StackCreate(2, 2); 
    int d[5] = {1, 2, 3, 4, 5};
    int* out;
    size_t capPeak, capAfterShrink;
    
    /* Push 5 items. Capacity grows 2 -> 4 -> 6 */
    StackPush(s, &d[0]);
    StackPush(s, &d[1]);
    StackPush(s, &d[2]);
    StackPush(s, &d[3]);
    StackPush(s, &d[4]);
    
    capPeak = StackCapacity(s); /* Should be 6 */
    
    /* Pop items to trigger shrink.
       Currently 5 items, cap 6.
       Pop to 4 items -> Empty space is 2 (1 block). No shrink (requires 2 empty blocks).
       Pop to 3 items -> Empty space is 3. No shrink.
       Pop to 2 items -> Empty space is 4 (2 blocks). Shrink triggered! Cap goes to 4. */
    StackPop(s, (void**)&out);
    StackPop(s, (void**)&out);
    StackPop(s, (void**)&out);
    
    capAfterShrink = StackCapacity(s);
    
    PrintTestResult("Stack Peak Capacity Correct", capPeak == 6);
    PrintTestResult("Stack Shrinks dynamically (Hysteresis working)", capAfterShrink == 4);
    
    StackDestroy(&s, NULL);
}

static void TestStackPrint(void)
{
    Stack* s = StackCreate(5, 2);
    int d[3] = {11, 22, 33};
    size_t count;
    
    StackPush(s, &d[0]);
    StackPush(s, &d[1]);
    StackPush(s, &d[2]);
    
    printf("    Print Output (Top to Bottom): ");
    count = StackPrint(s, PrintIntAction, NULL);
    printf("\n");
    
    PrintTestResult("StackPrint counts correct iterations", count == 3);
    StackDestroy(&s, NULL);
}

/* ========================================================== */
/* Main                                                       */
/* ========================================================== */

int main(void)
{
    printf("--- Stack Test Suite ---\n");
    TestStackCreateAndDestroy();
    TestStackPushAndTop();
    TestStackFixedOverflow();
    TestStackDynamicGrowth();
    TestStackDynamicShrink();
    TestStackPrint();
    
    return 0;
}