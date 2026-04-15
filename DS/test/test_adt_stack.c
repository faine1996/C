#include <stdio.h>
#include "../inc/adt_stack.h"

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

/* ========================================================== */
/* Test Cases                                                 */
/* ========================================================== */

static void TestStackCreateAndDestroy(void)
{
    Stack* s = StackCreate(5);
    PrintTestResult("StackCreate valid sizes", s != NULL);
    
    StackDestroy(&s);
    PrintTestResult("StackDestroy sets pointer to NULL", s == NULL);
}

static void TestStackPushAndPop(void)
{
    Stack* s = StackCreate(5);
    int dataIn = 42;
    int dataOut = 0;
    StackResult resPush;
    StackResult resPop;
    
    resPush = StackPush(s, dataIn);
    resPop = StackPop(s, &dataOut);
    
    PrintTestResult("StackPush returns SUCCESS", resPush == STACK_SUCCESS);
    PrintTestResult("StackPop fetches correct data", resPop == STACK_SUCCESS && dataOut == 42);
    
    StackDestroy(&s);
}

static void TestStackOverflow(void)
{
    Stack* s = StackCreate(2); 
    StackResult res;
    
    StackPush(s, 10);
    StackPush(s, 20);
    res = StackPush(s, 30); /* Capacity is 2, should fail */
    
    PrintTestResult("Stack Overflow Handled", res == STACK_OVERFLOW_ERROR);
    StackDestroy(&s);
}

static void TestStackUnderflow(void)
{
    Stack* s = StackCreate(2); 
    int out;
    StackResult res;
    
    res = StackPop(s, &out); /* Empty stack, should fail */
    
    PrintTestResult("Stack Underflow (Empty) Handled", res == STACK_DATA_NOT_FOUND_ERROR);
    StackDestroy(&s);
}

static void TestStackIsEmpty(void)
{
    Stack* s = StackCreate(2); 
    int out;
    size_t emptyBefore, emptyAfterPush, emptyAfterPop;
    
    emptyBefore = StackIsEmpty(s);
    StackPush(s, 10);
    emptyAfterPush = StackIsEmpty(s);
    StackPop(s, &out);
    emptyAfterPop = StackIsEmpty(s);
    
    PrintTestResult("StackIsEmpty evaluates correctly", emptyBefore == 1 && emptyAfterPush == 0 && emptyAfterPop == 1);
    StackDestroy(&s);
}

/* ========================================================== */
/* Main                                                       */
/* ========================================================== */

int main(void)
{
    printf("--- ADT Integer Stack Test Suite ---\n");
    TestStackCreateAndDestroy();
    TestStackPushAndPop();
    TestStackOverflow();
    TestStackUnderflow();
    TestStackIsEmpty();
    
    return 0;
}