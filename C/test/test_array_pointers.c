#include <stdio.h>

#include "../inc/array_pointers.h"

void testClearWinner();
void testTieBreaker();
void testNoDuplicates();
void testEmptyArray();

int main() {
    printf("--- Running Array Tests ---\n");
    
    testClearWinner();
    testTieBreaker();
    testNoDuplicates();
    testEmptyArray();
    
    printf("--- All Tests Complete ---\n");
    return 0;
}

void testClearWinner() {
    int arr[] = {1, 3, 7, 3, 2, 3, 7};
    int result = maxNumberApp(arr, 7);
    printf("Test 1 (Clear Winner): Expected 3, Got %d\n", result);
}

void testTieBreaker() {
    int arr[] = {10, 20, 30, 20, 10};
    int result = maxNumberApp(arr, 5);
    printf("Test 2 (Tie-Breaker): Expected 10, Got %d\n", result);
}

void testNoDuplicates() {
    int arr[] = {5, 9, 2, 4};
    int result = maxNumberApp(arr, 4);
    printf("Test 3 (No Duplicates): Expected 5, Got %d\n", result);
}

void testEmptyArray() {
    int result = maxNumberApp(NULL, 0);
    printf("Test 4 (Empty/Null): Expected %d, Got %d\n", ERROR, result);
}