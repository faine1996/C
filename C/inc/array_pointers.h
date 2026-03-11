#ifndef ARRAY_POINTERS_H
#define ARRAY_POINTERS_H

#include <stdio.h>

#define ERROR -9999999
#define SUCCESS 0

/*
 * Description: A functoin that returns the number which appears most often in an array
 * Expected Input: array of numbers (int), length of array (size_t).
 * Expected Output: Returns the number (int) which appears most often.
 * Return: SUCCESS, or ERROR if n is out of bounds.
 */

int maxNumberApp(int *numbers, size_t size, int *result);

/*
 * Description: A functoin that returns the number of even numbers in a given array and puts even numbers first and odd numbers second
 * Expected Input: array of numbers (int), length of array (size_t).
 * Expected Output: Returns the total number of even numbers in the given array.
 * Return: SUCCESS, or ERROR if n is out of bounds.
 */

int organizeEvenOdd(int *numbers, size_t size, int *even_count);

/*
 * Description: A functoin that returns an array sorted in ascending order
 * Expected Input: array of numbers (int), length of array (size_t).
 * Expected Output: Returns an array sorted in ascending order
 * Return: SUCCESS, or ERROR if n is out of bounds.
 */
int bubbleSort(int numbers[], size_t size);

/*
 * description: sorts an array of 0s and 1s so all 0s appear first.
 * expected input: array of numbers (int *), length of array (size_t).
 * expected output: array modified in place.
 * return: SUCCESS, or ERROR if input is invalid.
 */
int sortBinaryArray(int *numbers, size_t size);

 #endif