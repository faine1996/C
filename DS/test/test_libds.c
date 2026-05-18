#include <stdio.h>
#include "../inc/ds.h" /* Make sure this file includes "stack.h" */

int main(void)
{
    Stack* myStack = NULL;
    int val1 = 42;
    int val2 = 99;
    int* poppedValue = NULL;

    /* Generic stack takes: (initialCapacity, blockSize) */
    myStack = StackCreate(10, 5);

    if (NULL == myStack)
    {
        printf("Library linkage failed: Could not create stack\n");
        return 1;
    }

    /* Push addresses of the local integers (cast to void*) */
    StackPush(myStack, (void*)&val1);
    StackPush(myStack, (void*)&val2);

    /* Pop the value back into our pointer */
    StackPop(myStack, (void**)&poppedValue);

    if (NULL != poppedValue && 99 == *poppedValue)
    {
        printf("Success: Dynamic library is fully operational. Popped value: %d\n", *poppedValue);
    }
    else
    {
        printf("Failure: Unexpected value popped.\n");
    }

    /* Destroy the stack. Pass NULL because our integers are on the call stack, not malloc'd */
    StackDestroy(&myStack, NULL);

    return 0;
}