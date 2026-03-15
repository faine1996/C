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
void testAToI_Str1(void);
void testAToI_Str2(void);
void testAToI_Str3(void);
void testAToI_Str4(void);
void testAToI_Str5(void);
void testAToI_Str6(void);
void testAToI_Str7(void);
void testIToA_Positive(void);
void testIToA_Negative(void);
void testIToA_Zero(void);
void testIToA_MaxInt(void);
void testIToA_MinInt(void);
void testIToA_NullBuffer(void);

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

    testAToI_Str1();
    testAToI_Str2();
    testAToI_Str3();
    testAToI_Str4();
    testAToI_Str5();
    testAToI_Str6();
    testAToI_Str7();

    testIToA_Positive();
    testIToA_Negative();
    testIToA_Zero();
    testIToA_MaxInt();
    testIToA_MinInt();
    testIToA_NullBuffer();
    
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

void testAToI_Str1(void) 
{
    char str[] = "1024";
    int result = 0;
    int status;

    printf("Test str1: Original: '%s'\n", str);
    status = MyAToI(str, &result);
    printf("Expected: 1024\n");
    printf("Got     : %d (Status: %d)\n\n", result, status);
}

void testAToI_Str2(void) 
{
    char str[] = "3.14";
    int result = 0;
    int status;

    printf("Test str2: Original: '%s'\n", str);
    status = MyAToI(str, &result);

    printf("Expected: 3\n"); 
    printf("Got     : %d (Status: %d)\n\n", result, status);
}

void testAToI_Str3(void) 
{
    char str[] = " 123 55";
    int result = 0;
    int status;

    printf("Test str3: Original: '%s'\n", str);
    status = MyAToI(str, &result);

    printf("Expected: 123\n");
    printf("Got     : %d (Status: %d)\n\n", result, status);
}

void testAToI_Str4(void) 
{
    char str[] = "  +44";
    int result = 0;
    int status;

    printf("Test str4: Original: '%s'\n", str);
    status = MyAToI(str, &result);
    printf("Expected: 44\n");
    printf("Got     : %d (Status: %d)\n\n", result, status);
}

void testAToI_Str5(void) 
{
    char str[] = " -8 44";
    int result = 0;
    int status;

    printf("Test str5: Original: '%s'\n", str);
    status = MyAToI(str, &result);

    printf("Expected: -8\n");
    printf("Got     : %d (Status: %d)\n\n", result, status);
}

void testAToI_Str6(void) 
{
    char str[] = "  67abc";
    int result = 0;
    int status;

    printf("Test str6: Original: '%s'\n", str);
    status = MyAToI(str, &result);

    printf("Expected: 67\n");
    printf("Got     : %d (Status: %d)\n\n", result, status);
}

void testAToI_Str7(void) 
{
    char str[] = "abc987";
    int result = -999;
    int status;

    printf("Test str7: Original: '%s'\n", str);
    status = MyAToI(str, &result);

    printf("Expected: 0\n"); 
    printf("Got     : %d (Status: %d)\n\n", result, status);
}

void testIToA_Positive(void) 
{
    int num = 12345;
    char buffer[32] = {0}; /* 32 bytes is plenty for a 32-bit integer */
    int status;

    printf("Test 1 (Positive Number): Original: %d\n", num);
    status = MyIToA(num, buffer);
    printf("Expected: '12345'\n");
    printf("Got     : '%s' (Status: %d)\n\n", buffer, status);
}

void testIToA_Negative(void) 
{
    int num = -9876;
    char buffer[32] = {0};
    int status;

    printf("Test 2 (Negative Number): Original: %d\n", num);
    status = MyIToA(num, buffer);
    printf("Expected: '-9876'\n");
    printf("Got     : '%s' (Status: %d)\n\n", buffer, status);
}

void testIToA_Zero(void) 
{
    int num = 0;
    char buffer[32] = {0};
    int status;

    printf("Test 3 (Zero): Original: %d\n", num);
    status = MyIToA(num, buffer);
    printf("Expected: '0'\n");
    printf("Got     : '%s' (Status: %d)\n\n", buffer, status);
}

void testIToA_MaxInt(void) 
{
    int num = 2147483647; /* INT_MAX */
    char buffer[32] = {0};
    int status;

    printf("Test 4 (Maximum Integer): Original: %d\n", num);
    status = MyIToA(num, buffer);
    printf("Expected: '2147483647'\n");
    printf("Got     : '%s' (Status: %d)\n\n", buffer, status);
}

void testIToA_MinInt(void) 
{
    int num = -2147483648; /* INT_MIN */
    char buffer[32] = {0};
    int status;

    printf("Test 5 (Minimum Integer): Original: %d\n", num);
    status = MyIToA(num, buffer);
    /* If this fails, it means your code tried to do `temp = -_num;` and overflowed! */
    printf("Expected: '-2147483648'\n");
    printf("Got     : '%s' (Status: %d)\n\n", buffer, status);
}

void testIToA_NullBuffer(void) 
{
    int num = 100;
    int status;

    printf("Test 6 (NULL Buffer): Original: %d\n", num);
    status = MyIToA(num, NULL);
    printf("Expected Status: %d\n", ERROR);
    printf("Got Status     : %d\n\n", status);
}