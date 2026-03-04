#include <stdio.h>
#include "../inc/factorial.h"

int test_normal_factorial(void);
int test_zero_factorial(void);
int test_negative_factorial(void);

int main(void) 
{
    int passed = 0;
    int total = 3;

    printf("--- Running Factorial Tests ---\n");

    passed += test_normal_factorial();
    passed += test_zero_factorial();
    passed += test_negative_factorial();

    printf("-------------------------------\n");
    printf("Results: %d/%d tests passed.\n", passed, total);
    
    return 0;
}

int test_normal_factorial(void) 
{
    unsigned long expected = 120;
    unsigned long result = calculateFactorial(5);
    
    if (result == expected) 
    {
        printf("[PASS] test_normal_factorial\n");
        return 1;
    } 
    else 
    {
        printf("[FAIL] test_normal_factorial: expected %lu, got %lu\n", expected, result);
        return 0;
    }
}

int test_zero_factorial(void) 
{
    unsigned long expected = 1;
    unsigned long result = calculateFactorial(0);
    
    if (result == expected) 
    {
        printf("[PASS] test_zero_factorial\n");
        return 1;
    } 
    else 
    {
        printf("[FAIL] test_zero_factorial: expected %lu, got %lu\n", expected, result);
        return 0;
    }
}

int test_negative_factorial(void) 
{
    unsigned long expected = 0;
    unsigned long result = calculateFactorial(-5);
    
    if (result == expected) 
    {
        printf("[PASS] test_negative_factorial\n");
        return 1;
    } 
    else 
    {
        printf("[FAIL] test_negative_factorial: expected %lu, got %lu\n", expected, result);
        return 0;
    }
}