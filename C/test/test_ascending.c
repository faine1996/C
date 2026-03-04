#include <stdio.h>
#include "../inc/ascending.h"

int test_normal_ascending(void);
int test_zero_ascending(void);
int test_negative_ascending(void);

int main(void) 
{
    int passed = 0;
    int total = 3;

    printf("--- Running ascending Tests ---\n");

    passed += test_normal_ascending();
    passed += test_zero_ascending();
    passed += test_negative_ascending();

    printf("-------------------------------\n");
    printf("Results: %d/%d tests passed.\n", passed, total);
    
    return 0;
}

int test_normal_ascending(void) 
{
    int result = isAscending(14455668);
    
    if (result) 
    {
        printf("[PASS] test_normal_ascending\n");
        return 1;
    } 
    else 
    {
        printf("[FAIL] test_normal_ascending\n");
        return 0;
    }
}

int test_zero_ascending(void) 
{
    int result = isAscending(0);
    
    if (result) 
    {
        printf("[PASS] test_zero_ascending\n");
        return 1;
    } 
    else 
    {
        printf("[FAIL] test_zero_ascending\n");
        return 0;
    }
}

int test_negative_ascending(void) 
{
    int result = isAscending(-12345);
    
    if (result) 
    {
        printf("[PASS] test_negative_ascending\n");
        return 1;
    } 
    else 
    {
        printf("[FAIL] test_negative_ascending\n");
        return 0;
    }
}