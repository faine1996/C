#include <stdio.h>
#include "../inc/reverse_num.h"

int test_normal_reverse_num(void);
int test_zero_reverse_num(void);
int test_negative_reverse_num(void);

int main(void) 
{
    int passed = 0;
    int total = 3;

    printf("--- Running Reverse Number Tests ---\n");

    passed += test_normal_reverse_num();
    passed += test_zero_reverse_num();
    passed += test_negative_reverse_num();

    printf("-------------------------------\n");
    printf("Results: %d/%d tests passed.\n", passed, total);
    
    return 0;
}

int test_normal_reverse_num(void) 
{
    int result = is_reverse_num(1234567);
    
    if (7654321 == result) 
    {
        printf("[PASS] test_normal_reverse_num\n");
        return 1;
    } 
    else 
    {
        printf("[FAIL] test_normal_reverse_num\n");
        return 0;
    }
}

int test_zero_reverse_num(void) 
{
    int result = is_reverse_num(0);
    
    if (!result) 
    {
        printf("[PASS] test_zero_reverse_num\n");
        return 1;
    } 
    else 
    {
        printf("[FAIL] test_zero_reverse_num\n");
        return 0;
    }
}

int test_negative_reverse_num(void) 
{
    int result = is_reverse_num(-87654);
    
    if (-45678 == result) 
    {
        printf("[PASS] test_negative_reverse_num\n");
        return 1;
    } 
    else 
    {
        printf("[FAIL] test_negative_reverse_num\n");
        return 0;
    }
}