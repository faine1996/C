#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../inc/new_gen_dlist.h"

/* --- Helper Action --- */
int PrintInt(void* _element, void* _context)
{
    (void)_context;
    printf("%d ", *(int*)_element);
    return 1;
}

void Test_BasicDList(void)
{
    List* list;
    int a = 10, b = 20;
    void* popped;

    list = ListCreate();
    assert(NULL != list);
    assert(1 == ListIsEmpty(list));

    ListPushHead(list, &a);
    ListPushTail(list, &b);
    assert(0 == ListIsEmpty(list));

    popped = ListPopHead(list);
    assert(10 == *(int*)popped);

    ListDestroy(&list, NULL);
    assert(NULL == list);

    printf("  [PASS] Basic DList Operations\n");
}

int main(void)
{
    printf("Starting Generic DList Test Suite...\n");
    Test_BasicDList();
    printf("All DList tests passed!\n");
    return 0;
}