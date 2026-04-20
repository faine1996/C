#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../inc/person_list.h"

/* Test Function Declarations */
void Test_InsertHead(void);
void Test_InsertByKey_Sorting(void);
void Test_DuplicatePrevention(void);
void Test_RemoveByKey(void);
void Test_GetLastNodeRecursive(void);
static Person* CreatePerson(int _id, const char* _name);

int main(void)
{
    printf("Starting Linked List Test Suite...\n");

    Test_InsertHead();
    Test_InsertByKey_Sorting();
    Test_DuplicatePrevention();
    Test_RemoveByKey();
    Test_GetLastNodeRecursive();

    printf("All Linked List tests passed!\n");

    return 0;
}

/* Test Function Definitions */

void Test_InsertHead(void)
{
    Person* head;
    Person* p1;
    Person* p2;
    Person* item;

    head = NULL;
    p1 = CreatePerson(101, "Faine");
    p2 = CreatePerson(102, "Angel");

    /* Test insertion */
    head = ListInsertHead(head, p1);
    head = ListInsertHead(head, p2);

    /* Yoda check: head should now be p2 */
    assert(p2 == head);
    assert(p1 == head->next);

    /* Cleanup for next test */
    head = ListRemoveHead(head, &item);
    free(item);
    head = ListRemoveHead(head, &item);
    free(item);

    printf("  [PASS] Test_InsertHead\n");
}

void Test_InsertByKey_Sorting(void)
{
    Person* head;
    Person* p_mid;
    Person* p_low;
    Person* p_high;

    head = NULL;
    p_mid = CreatePerson(50, "Middle");
    p_low = CreatePerson(10, "Low");
    p_high = CreatePerson(90, "High");

    /* Insert out of order to test sorting */
    head = ListInsertByKey(head, 50, p_mid);
    head = ListInsertByKey(head, 90, p_high);
    head = ListInsertByKey(head, 10, p_low);

    /* Verify order: 10 -> 50 -> 90 */
    assert(10 == head->id);
    assert(50 == head->next->id);
    assert(90 == head->next->next->id);

    /* Manual cleanup */
    free(p_low);
    free(p_mid);
    free(p_high);

    printf("  [PASS] Test_InsertByKey_Sorting\n");
}

void Test_DuplicatePrevention(void)
{
    Person* head;
    Person* p1;
    Person* p2;

    head = NULL;
    p1 = CreatePerson(200, "Original");
    p2 = CreatePerson(200, "Duplicate");

    head = ListInsertByKey(head, 200, p1);
    
    /* Attempting to insert same ID should fail and return original head */
    head = ListInsertByKey(head, 200, p2);

    /* verify list only has one element */
    assert(NULL == head->next);
    
    free(p1);
    free(p2);

    printf("  [PASS] Test_DuplicatePrevention\n");
}

void Test_RemoveByKey(void)
{
    Person* head;
    Person* p1;
    Person* p2;
    Person* removed;

    head = NULL;
    p1 = CreatePerson(1, "Alpha");
    p2 = CreatePerson(2, "Beta");

    head = ListInsertHead(head, p1);
    head = ListInsertHead(head, p2);

    /* Remove Alpha (id: 1) */
    head = ListRemoveByKey(head, 1, &removed);

    assert(p1 == removed);
    assert(0 == strcmp(removed->name, "Alpha"));
    assert(NULL == head->next); /* Only Beta left */

    free(p1);
    free(p2);

    printf("  [PASS] Test_RemoveByKey\n");
}

void Test_GetLastNodeRecursive(void)
{
    Person* head;
    Person* last;
    Person* p1;
    Person* p2;
    Person* p3;

    head = NULL;

    /* Case 1: Empty List - Should return NULL */
    last = GetLastNodeRecursive(head);
    assert(NULL == last);

    /* Case 2: Single Node - Should return itself */
    p1 = CreatePerson(10, "Solitary");
    head = ListInsertHead(head, p1);
    
    last = GetLastNodeRecursive(head);
    assert(p1 == last);
    assert(10 == last->id);

    /* Case 3: Multiple Nodes - Should find the original tail */
    /* List currently: 10 */
    p2 = CreatePerson(20, "Middle");
    p3 = CreatePerson(30, "NewHead");
    
    head = ListInsertHead(head, p2); /* List: 20 -> 10 */
    head = ListInsertHead(head, p3); /* List: 30 -> 20 -> 10 */

    last = GetLastNodeRecursive(head);
    
    /* Yoda comparison to check if we found p1 (the first one added) */
    assert(p1 == last);
    assert(0 == strcmp(last->name, "Solitary"));

    /* Cleanup: User-allocated data in the Heap must be freed  */
    free(p1);
    free(p2);
    free(p3);

    printf("  [PASS] Test_GetLastNodeRecursive\n");
}

/* Helper Definitions */

static Person* CreatePerson(int _id, const char* _name)
{
    /* Allocating in the Heap segment  */
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