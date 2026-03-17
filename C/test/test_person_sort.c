#include <stdio.h>
#include "../inc/person_sort.h"

void testSortByIdValid(void);
void testSortByIdNull(void);
void testSortByNameValid(void);

int main(void)
{
    printf("--- Running Struct Sort Tests ---\n\n");
    
    testSortByIdValid();
    testSortByIdNull();
    testSortByNameValid();
    
    printf("\n--- All Tests Complete ---\n");
    return 0;
}

void testSortByIdValid(void)
{
    int status;
    struct Person people[3] = 
    {
        {105, "Charlie", 22},
        {101, "Alice", 30},
        {108, "Bob", 25}
    };

    printf("Test 1 (Sort By ID Valid):\n");
    status = SortPersonsById(people, 3);
    
    printf("Expected Order: 101 (Alice), 105 (Charlie), 108 (Bob)\n");
    printf("Got Order     : %d (%s), %d (%s), %d (%s) \nStatus: %d\n\n", 
            people[0].id, people[0].name,
            people[1].id, people[1].name,
            people[2].id, people[2].name, status);
}

void testSortByIdNull(void)
{
    int status;

    printf("Test 2 (Sort By ID NULL Pointer):\n");
    status = SortPersonsById(NULL, 3);
    
    printf("Expected Status: %d\n", ERROR);
    printf("Got Status     : %d\n\n", status);
}

void testSortByNameValid(void)
{
    int status;
    struct Person people[3] = 
    {
        {102, "Zack", 29},
        {101, "Alice", 30},
        {103, "Charlie", 22}
    };

    printf("Test 3 (Sort By Name Valid):\n");
    status = SortPersonsByName(people, 3);
    
    printf("Expected Order: Alice, Charlie, Zack\n");
    printf("Got Order     : %s, %s, %s \nStatus: %d\n\n", 
            people[0].name, people[1].name, people[2].name, status);
}