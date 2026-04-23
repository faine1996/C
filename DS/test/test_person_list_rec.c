#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../inc/person_list_rec.h"

/* --- Helper Function Declarations --- */
static Person* CreateTestPerson(int _id, const char* _name);

/* --- Main Test Functions --- */

void Test_HeadOperations(void)
{
    Person* head = NULL;
    Person* p1 = CreateTestPerson(1, "A");
    Person* removed = NULL;

    head = ListInsertHead(head, p1);
    assert(p1 == head);

    head = ListRemoveHead(head, &removed);
    assert(NULL == head);
    assert(p1 == removed);

    free(removed);
    printf("  [PASS] Head Operations\n");
}

void Test_KeyOperations(void)
{
    Person* head = NULL;
    Person* p1 = CreateTestPerson(0, "B");
    Person* p2 = CreateTestPerson(0, "C");
    Person* rem = NULL;

    /* Iterative Insert */
    head = ListInsertByKey(head, 20, p1);
    /* Recursive Insert */
    head = ListInsertByKeyRec(head, 10, p2);

    assert(10 == head->id);
    assert(20 == head->next->id);

    /* Recursive Remove */
    head = ListRemoveByKeyRec(head, 10, &rem);
    assert(20 == head->id);
    free(rem);

    /* Iterative Remove */
    head = ListRemoveByKey(head, 20, &rem);
    assert(NULL == head);
    free(rem);

    printf("  [PASS] Key Operations (Iterative & Recursive)\n");
}

int main(void)
{
    printf("Starting Integrated Person List Tests...\n");
    Test_HeadOperations();
    Test_KeyOperations();
    printf("All tests passed!\n");
    return 0;
}

/* --- Helper Function Definitions --- */

static Person* CreateTestPerson(int _id, const char* _name)
{
    Person* p;
    p = (Person*)malloc(sizeof(Person));
    if (NULL != p)
    {
        p->id = _id;
        strcpy(p->name, _name);
        p->next = NULL;
    }
    return p;
}