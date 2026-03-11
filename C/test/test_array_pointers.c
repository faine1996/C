#include <stdio.h>
#include <stdlib.h>

#include "../inc/array_pointers.h"

void testClearWinner();
void testTieBreaker();
void testNoDuplicates();
void testEmptyArray();
void testMixedNumbers();
void testNegativeNumbers();
void testAllOdd();
void testAllEven();
void testBubbleAlreadySorted();
void testBubbleReverseOrder();
void testBubbleMixedNumbers();
void testBubbleWithDuplicates();
void testBubbleEmptyArray();
void printArray(int arr[], size_t size);

int main() 
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
    
    printf("--- All Tests Complete ---\n");
    return 0;
}

void testClearWinner() 
{
    int arr[] = {1, 3, 7, 3, 2, 3, 7};
    int result = maxNumberApp(arr, 7);
    printf("Test 1 (Clear Winner): Expected 3, Got %d\n", result);
}

void testTieBreaker() 
{
    int arr[] = {10, 20, 30, 20, 10};
    int result = maxNumberApp(arr, 5);
    printf("Test 2 (Tie-Breaker): Expected 10, Got %d\n", result);
}

void testNoDuplicates() 
{
    int arr[] = {5, 9, 2, 4};
    int result = maxNumberApp(arr, 4);
    printf("Test 3 (No Duplicates): Expected 5, Got %d\n", result);
}

void testEmptyArray() 
{
    int result = maxNumberApp(NULL, 0);
    printf("Test 4 (Empty/Null): Expected %d, Got %d\n", ERROR, result);
}

void testMixedNumbers() 
{
    int arr[] = {5, 2, 8, 3, 4, 1};
    size_t size = 6;
    size_t i = 0;
    int count = 0;
    printf("Test Mixed: Original: { ");
    for(; i < size; ++i) printf("%d ", arr[i]);
    
    count = organizeEvenOdd(arr, size);
    
    i = 0;
    printf("}. Result (Count: %d): { ", count);
    for(; i < size; ++i) printf("%d ", arr[i]);
    printf("}\n");
}

void testNegativeNumbers() 
{
    int arr[] = {-2, -3, 4, -5, 0};
    size_t size = 5;
    size_t i = 0;
    int count = 0;
    printf("Test Negative: Original: { ");
    for(; i < size; ++i) printf("%d ", arr[i]);
    
    count = organizeEvenOdd(arr, size);
    
    i = 0;
    printf("}. Result (Count: %d): { ", count);
    for(; i < size; ++i) printf("%d ", arr[i]);
    printf("}\n");
}

void testAllOdd() 
{
    int arr[] = {1, 3, 5};
    size_t size = 3;
    size_t i = 0;
    int count = 0;
    printf("Test All Odd: Original: { ");
    for(; i < size; ++i) printf("%d ", arr[i]);
    
    count = organizeEvenOdd(arr, size);
    i = 0;
    printf("}. Result (Count: %d): { ", count);
    for(; i < size; ++i) printf("%d ", arr[i]);
    printf("}\n");
}

void testAllEven() 
{
    int arr[] = {2, 4, 6};
    size_t size = 3;
    size_t i = 0;
    int count = 0;
    printf("Test All Even: Original: { ");
    for(; i < size; ++i) printf("%d ", arr[i]);
    
    count = organizeEvenOdd(arr, size);
    i = 0;
    printf("}. Result (Count: %d): { ", count);
    for(; i < size; ++i) printf("%d ", arr[i]);
    printf("}\n");
}

void printArray(int arr[], size_t size) 
{
    size_t i;
    printf("{ ");
    for(i = 0; i < size; ++i) 
    {
        printf("%d ", arr[i]);
    }
    printf("}");
}

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