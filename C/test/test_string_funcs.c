#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/string_funcs.h" 

void testReverseEvenLengthMixedCase(void);
void testReverseOddLengthMixedCase(void);
void testReverseAlreadyLower(void);
void testReverseSingleCharCapital(void);
void testReverseEmptyString(void);
void testReverseNullPointer(void);
void testPalindromeValidEven(void);
void testPalindromeValidOdd(void);
void testPalindromeInvalid(void);

int main(void) 
{
    printf("--- Running ReverseStr & Lowercase Tests ---\n\n");
    
    testReverseEvenLengthMixedCase();
    testReverseOddLengthMixedCase();
    testReverseAlreadyLower();
    testReverseSingleCharCapital();
    testReverseEmptyString();
    testReverseNullPointer();

    testPalindromeValidEven();
    testPalindromeValidOdd();
    testPalindromeInvalid();
    
    printf("\n--- All Tests Complete ---\n");
    return 0;
}


void testReverseEvenLengthMixedCase(void) 
{
    char str[] = "HeLlO!"; 
    int status;

    printf("Test 1 (Even Length, Mixed): Original: 'HeLlO!'.\n");
    status = ReverseStr(str);
    printf("Expected: '!olleh'\n");
    printf("Got     : '%s' (Status: %d)\n\n", str, status);
}

void testReverseOddLengthMixedCase(void) 
{
    char str[] = "WoRlD"; 
    int status;

    printf("Test 2 (Odd Length, Mixed): Original: 'WoRlD'.\n");
    status = ReverseStr(str);
    printf("Expected: 'dlrow'\n");
    printf("Got     : '%s' (Status: %d)\n\n", str, status);
}

void testReverseAlreadyLower(void) 
{
    char str[] = "racecar"; 
    int status;

    printf("Test 3 (Already Lowercase): Original: 'racecar'.\n");
    status = ReverseStr(str);
    printf("Expected: 'racecar'\n");
    printf("Got     : '%s' (Status: %d)\n\n", str, status);
}

void testReverseSingleCharCapital(void) 
{
    char str[] = "A"; 
    int status;

    printf("Test 4 (Single Capital Char): Original: 'A'.\n");
    status = ReverseStr(str);
    printf("Expected: 'a'\n");
    printf("Got     : '%s' (Status: %d)\n\n", str, status);
}

void testReverseEmptyString(void) 
{
    char str[] = "";
    int status;

    printf("Test 5 (Empty String): Original: ''.\n");
    status = ReverseStr(str);
    printf("Expected: ''\n");
    printf("Got     : '%s' (Status: %d)\n\n", str, status);
}

void testReverseNullPointer(void) 
{
    int status;

    printf("Test 6 (NULL Pointer):\n");
    status = ReverseStr(NULL);
    printf("Expected Status: %d\n", ERROR);
    printf("Got Status     : %d\n\n", status);
}

void testPalindromeValidEven(void) 
{
    char str[] = "abba";
    int result;
    
    printf("Test Palindrome Even: Checking '%s'. ", str);
    result = !IsPalindrome(str);
    printf("Expected: 1, Got: %d\n", result);
}

void testPalindromeValidOdd(void) 
{
    char str[] = "racecar";
    int result;
    
    printf("Test Palindrome Odd: Checking '%s'. ", str);
    result = !IsPalindrome(str);
    printf("Expected: 1, Got: %d\n", result);
}

void testPalindromeInvalid(void) 
{
    char str[] = "hello";
    int result;
    
    printf("Test Palindrome Invalid: Checking '%s'. ", str);
    result = !IsPalindrome(str);
    printf("Expected: 0, Got: %d\n", result);
}