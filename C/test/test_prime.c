#include <stdio.h>

#include "../inc/prime.h"

int test_isPrime(int num);

int main(void)
{
    int passed = 0;
    int total = 4;

    printf("--- Running ascending Tests ---\n");

    passed += test_is_prime();
    passed += test_is_not_prime();
    passed += test_edge_case_is_prime();
    passed += test_edge_case2_is_prime();

    printf("-------------------------------\n");
    printf("Results: %d/%d tests passed.\n", passed, total);

    return 0;
}

int test_is_prime()
{
    int result = isPrime(4073);
    if (result) 
    {
        printf("[PASS] test_is_prime\n");
        return 1;
    } 
    else 
    {
        printf("[FAIL] test_is_prime\n");
        return 0;
    }
}

int test_is_not_prime()
{
    int result = isPrime(602);
    if (!result) 
    {
        printf("[PASS] test_is_not_prime\n");
        return 1;
    } 
    else 
    {
        printf("[FAIL] test_is_not_prime\n");
        return 0;
    }
}

int test_edge_case_is_prime()
{
    int result = isPrime(-100);
    if (!result) 
    {
        printf("[PASS] test_edge_case_is_prime\n");
        return 1;
    } 
    else 
    {
        printf("[FAIL] test_edge_case_is_prime\n");
        return 0;
    }
}

int test_edge_case2_is_prime()
{
    int result = isPrime(2);
    if (result) 
    {
        printf("[PASS] test_edge_case2_is_prime\n");
        return 1;
    } 
    else 
    {
        printf("[FAIL] test_edge_case2_is_prime\n");
        return 0;
    }
}