#include <stdio.h>
#include <stddef.h>
#include "../inc/array_average.h"

int test_clean_integer_average(void);
int test_decimal_average(void);
int test_zero_size_protection(void);
int test_negative_numbers(void);

int main(void)
{
    int passed = 0;
    int total = 4;

    printf("--- Running Array Average Tests ---\n");

    passed += test_clean_integer_average();
    passed += test_decimal_average();
    passed += test_zero_size_protection();
    passed += test_negative_numbers();

    printf("-------------------------------\n");
    printf("Results: %d/%d tests passed.\n", passed, total);

    return 0;
}

int test_clean_integer_average(void)
{
    int arr[] = {2, 4, 6, 8, 10};
    float expected = 6.0f;
    float result = arrayAverage(arr, 5);

    if (expected == result)
    {
        printf("[PASS] test_clean_integer_average\n");
        return 1;
    }
    else
    {
        printf("[FAIL] test_clean_integer_average\n");
        return 0;
    }
}

int test_decimal_average(void)
{
    int arr[] = {1, 2};
    float expected = 1.5f;
    float result = arrayAverage(arr, 2);

    if (expected == result)
    {
        printf("[PASS] test_decimal_average\n");
        return 1;
    }
    else
    {
        printf("[FAIL] test_decimal_average\n");
        return 0;
    }
}

int test_zero_size_protection(void)
{
    int arr[] = {2, 4, 6, 8, 10};
    float expected = 0.0f;
    /* We pass a valid array, but tell the function the size is 0 */
    float result = arrayAverage(arr, 0);

    if (expected == result)
    {
        printf("[PASS] test_zero_size_protection\n");
        return 1;
    }
    else
    {
        printf("[FAIL] test_zero_size_protection\n");
        return 0;
    }
}

int test_negative_numbers(void)
{
    int arr[] = {-5, 15, -10};
    float expected = 0.0f;
    float result = arrayAverage(arr, 3);

    if (expected == result)
    {
        printf("[PASS] test_negative_numbers\n");
        return 1;
    }
    else
    {
        printf("[FAIL] test_negative_numbers\n");
        return 0;
    }
}