#include <stdio.h>
#include <stdlib.h>

#include "../inc/array_pointers.h"

void testClearWinner(void);
void testTieBreaker(void);
void testNoDuplicates(void);
void testEmptyArray(void);
void testMixedNumbers(void);
void testNegativeNumbers(void);
void testAllOdd(void);
void testAllEven(void);
void testBubbleAlreadySorted(void);
void testBubbleReverseOrder(void);
void testBubbleMixedNumbers(void);
void testBubbleWithDuplicates(void);
void testBubbleEmptyArray(void);
void testBinaryMixed(void);
void testBinaryAlreadySorted(void);
void testBinaryAllZeros(void);
void testBinaryAllOnes(void);
void printArray(int *arr, size_t size);

int main(void) 
{
    printf("--- Running Array Tests ---\n");
    
    testClearWinner();
    testTieBreaker();
    testNoDuplicates();
    testEmptyArray();
    testMixedNumbers();
    testNegativeNumbers();
    testAllOdd();
    testAllEven();
    testBubbleAlreadySorted();
    testBubbleReverseOrder();
    testBubbleMixedNumbers();
    testBubbleWithDuplicates();
    testBubbleEmptyArray();
    testBinaryMixed();
    testBinaryAlreadySorted();
    testBinaryAllZeros();
    testBinaryAllOnes();
    
    printf("--- All Tests Complete ---\n");
    return 0;
}

/* --- Max Number Tests --- */

void testClearWinner(void) 
{
    int arr[] = {1, 3, 7, 3, 2, 3, 7};
    int result;
    int status;

    status = maxNumberApp(arr, 7, &result);
    printf("Test 1 (Clear Winner): Expected 3, Got %d (Status: %d)\n", result, status);
}

void testTieBreaker(void) 
{
    int arr[] = {10, 20, 30, 20, 10};
    int result;
    int status;

    status = maxNumberApp(arr, 5, &result);
    printf("Test 2 (Tie-Breaker): Expected 10, Got %d (Status: %d)\n", result, status);
}

void testNoDuplicates(void) 
{
    int arr[] = {5, 9, 2, 4};
    int result;
    int status;

    status = maxNumberApp(arr, 4, &result);
    printf("Test 3 (No Duplicates): Expected 5, Got %d (Status: %d)\n", result, status);
}

void testEmptyArray(void) 
{
    int result;
    int status;

    status = maxNumberApp(NULL, 0, &result);
    printf("Test 4 (Empty/Null): Expected %d, Got %d (Status: %d)\n", ERROR, status, status);
}

/* --- Even/Odd Tests --- */

void testMixedNumbers(void) 
{
    int arr[] = {5, 2, 8, 3, 4, 1};
    size_t size = 6;
    size_t i;
    int count;
    int status;

    i = 0;
    count = 0;

    printf("Test Mixed: Original: { ");
    for(; i < size; ++i) printf("%d ", arr[i]);
    
    status = organizeEvenOdd(arr, size, &count);
    
    i = 0;
    printf("}. Result (Count: %d, Status: %d): { ", count, status);
    for(; i < size; ++i) printf("%d ", arr[i]);
    printf("}\n");
}

void testNegativeNumbers(void) 
{
    int arr[] = {-2, -3, 4, -5, 0};
    size_t size = 5;
    size_t i;
    int count;
    int status;

    i = 0;
    count = 0;

    printf("Test Negative: Original: { ");
    for(; i < size; ++i) printf("%d ", arr[i]);
    
    status = organizeEvenOdd(arr, size, &count);
    
    i = 0;
    printf("}. Result (Count: %d, Status: %d): { ", count, status);
    for(; i < size; ++i) printf("%d ", arr[i]);
    printf("}\n");
}

void testAllOdd(void) 
{
    int arr[] = {1, 3, 5};
    size_t size = 3;
    size_t i;
    int count;
    int status;

    i = 0;
    count = 0;

    printf("Test All Odd: Original: { ");
    for(; i < size; ++i) printf("%d ", arr[i]);
    
    status = organizeEvenOdd(arr, size, &count);
    
    i = 0;
    printf("}. Result (Count: %d, Status: %d): { ", count, status);
    for(; i < size; ++i) printf("%d ", arr[i]);
    printf("}\n");
}

void testAllEven(void) 
{
    int arr[] = {2, 4, 6};
    size_t size = 3;
    size_t i;
    int count;
    int status;

    i = 0;
    count = 0;

    printf("Test All Even: Original: { ");
    for(; i < size; ++i) printf("%d ", arr[i]);
    
    status = organizeEvenOdd(arr, size, &count);
    
    i = 0;
    printf("}. Result (Count: %d, Status: %d): { ", count, status);
    for(; i < size; ++i) printf("%d ", arr[i]);
    printf("}\n");
}

/* --- Bubble Sort Tests --- */

void testBubbleAlreadySorted(void) 
{
    int arr[] = {1, 2, 3, 4, 5};
    size_t size = 5;
    int status;
    
    printf("Test Sorted (Early Exit): Original: ");
    printArray(arr, size);
    
    status = bubbleSort(arr, size);
    
    printf(". Result (Status: %d): ", status);
    printArray(arr, size);
    printf("\n");
}

void testBubbleReverseOrder(void) 
{
    int arr[] = {5, 4, 3, 2, 1};
    size_t size = 5;
    int status;
    
    printf("Test Reverse (Worst Case): Original: ");
    printArray(arr, size);
    
    status = bubbleSort(arr, size);
    
    printf(". Result (Status: %d): ", status);
    printArray(arr, size);
    printf("\n");
}

void testBubbleMixedNumbers(void) 
{
    int arr[] = {7, 2, 9, 1, 5};
    size_t size = 5;
    int status;
    
    printf("Test Mixed: Original: ");
    printArray(arr, size);
    
    status = bubbleSort(arr, size);
    
    printf(". Result (Status: %d): ", status);
    printArray(arr, size);
    printf("\n");
}

void testBubbleWithDuplicates(void) 
{
    int arr[] = {3, 1, 4, 1, 5, 9, 3};
    size_t size = 7;
    int status;
    
    printf("Test Duplicates: Original: ");
    printArray(arr, size);
    
    status = bubbleSort(arr, size);
    
    printf(". Result (Status: %d): ", status);
    printArray(arr, size);
    printf("\n");
}

void testBubbleEmptyArray(void) 
{
    int status;
    printf("Test Empty/Null: ");
    status = bubbleSort(NULL, 0);
    printf("Expected %d, Got %d\n", ERROR, status);
}

/* --- Binary Array Tests --- */

void testBinaryMixed(void) 
{
    int arr[] = {1, 0, 1, 1, 0, 0, 1, 0};
    size_t size = 8;
    int status;
    
    printf("Test Binary Mixed: Original: ");
    printArray(arr, size);
    
    status = sortBinaryArray(arr, size);
    
    printf(". Result (Status: %d): ", status);
    printArray(arr, size);
    printf("\n");
}

void testBinaryAlreadySorted(void) 
{
    int arr[] = {0, 0, 0, 1, 1, 1};
    size_t size = 6;
    int status;
    
    printf("Test Binary Sorted: Original: ");
    printArray(arr, size);
    
    status = sortBinaryArray(arr, size);
    
    printf(". Result (Status: %d): ", status);
    printArray(arr, size);
    printf("\n");
}

void testBinaryAllZeros(void) 
{
    int arr[] = {0, 0, 0, 0};
    size_t size = 4;
    int status;
    
    printf("Test Binary All 0s: Original: ");
    printArray(arr, size);
    
    status = sortBinaryArray(arr, size);
    
    printf(". Result (Status: %d): ", status);
    printArray(arr, size);
    printf("\n");
}

void testBinaryAllOnes(void) 
{
    int arr[] = {1, 1, 1, 1};
    size_t size = 4;
    int status;
    
    printf("Test Binary All 1s: Original: ");
    printArray(arr, size);
    
    status = sortBinaryArray(arr, size);
    
    printf(". Result (Status: %d): ", status);
    printArray(arr, size);
    printf("\n");
}

/* --- Helpers --- */

void printArray(int *arr, size_t size) 
{
    size_t i;
    printf("{ ");
    for(i = 0; i < size; ++i) 
    {
        printf("%d ", arr[i]);
    }
    printf("}");
}