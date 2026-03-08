#include <stdio.h>
#include "../inc/mastermind.h"


int test_mastermind_example1(void);
int test_mastermind_example2(void);
int test_mastermind_all_exact(void);
int test_mastermind_no_matches(void);
int test_mastermind_error_handling(void);


int main(void) 
{
    int passed = 0;
    int total = 5;

    printf("--- Running Mastermind Tests ---\n");

    passed += test_mastermind_example1();
    passed += test_mastermind_example2();
    passed += test_mastermind_all_exact();
    passed += test_mastermind_no_matches();
    passed += test_mastermind_error_handling();

    printf("-------------------------------\n");
    printf("Results: %d/%d tests passed.\n", passed, total);
    
    return 0;
}

int test_mastermind_example1(void) 
{
    int expected = 4; 
    int result = mastermind(1234, 4321);
    
    if (result == expected) 
    {
        printf("[PASS] test_mastermind_example1\n");
        return 1;
    } 
    else 
    {
        printf("[FAIL] test_mastermind_example1: expected %d, got %d\n", expected, result);
        return 0;
    }
}

int test_mastermind_example2(void) 
{
    int expected = 22;
    int result = mastermind(2468, 2648);
    
    if (result == expected) 
    {
        printf("[PASS] test_mastermind_example2\n");
        return 1;
    } 
    else 
    {
        printf("[FAIL] test_mastermind_example2: expected %d, got %d\n", expected, result);
        return 0;
    }
}

int test_mastermind_all_exact(void) 
{
    int expected = 30;
    int result = mastermind(123, 123);
    
    if (result == expected) 
    {
        printf("[PASS] test_mastermind_all_exact\n");
        return 1;
    } 
    else 
    {
        printf("[FAIL] test_mastermind_all_exact: expected %d, got %d\n", expected, result);
        return 0;
    }
}

int test_mastermind_no_matches(void) 
{
    int expected = 0; 
    int result = mastermind(123, 456);
    
    if (result == expected) 
    {
        printf("[PASS] test_mastermind_no_matches\n");
        return 1;
    } 
    else 
    {
        printf("[FAIL] test_mastermind_no_matches: expected %d, got %d\n", expected, result);
        return 0;
    }
}

int test_mastermind_error_handling(void) 
{
    int expected = -1;
    int result = mastermind(-1234, 4321);
    
    if (result == expected) 
    {
        printf("[PASS] test_mastermind_error_handling\n");
        return 1;
    } 
    else 
    {
        printf("[FAIL] test_mastermind_error_handling: expected %d, got %d\n", expected, result);
        return 0;
    }
}