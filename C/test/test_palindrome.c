#include <stdio.h>
#include "../inc/palindrome.h"

int test_normal_palindrome(void);
int test_zero_palindrome(void);
int test_negative_palindrome(void);

int main(void) 
{
    int passed = 0;
    int total = 3;

    printf("--- Running Palindrome Tests ---\n");

    passed += test_normal_palindrome();
    passed += test_zero_palindrome();
    passed += test_negative_palindrome();

    printf("-------------------------------\n");
    printf("Results: %d/%d tests passed.\n", passed, total);
    
    return 0;
}

int test_normal_palindrome(void) 
{
    int result = isPalindrome(4567654);
    
    if (result) 
    {
        printf("[PASS] test_normal_palindrome\n");
        return 1;
    } 
    else 
    {
        printf("[FAIL] test_normal_palindrome\n");
        return 0;
    }
}

int test_zero_palindrome(void) 
{
    int result = isPalindrome(0);
    
    if (result) 
    {
        printf("[PASS] test_zero_palindrome\n");
        return 1;
    } 
    else 
    {
        printf("[FAIL] test_zero_palindrome\n");
        return 0;
    }
}

int test_negative_palindrome(void) 
{
    int result = isPalindrome(-5);
    
    if (!result) 
    {
        printf("[PASS] test_negative_palindrome\n");
        return 1;
    } 
    else 
    {
        printf("[FAIL] test_negative_palindrome\n");
        return 0;
    }
}