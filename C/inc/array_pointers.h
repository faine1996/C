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

int maxNumberApp(int numbers[], size_t size);

/*
 * Description: A functoin that returns the number of even numbers in a given array and puts even numbers first and odd numbers second
 * Expected Input: array of numbers (int), length of array (size_t).
 * Expected Output: Returns the total number of even numbers in the given array.
 * Return: SUCCESS, or ERROR if n is out of bounds.
 */

int organizeEvenOdd(int numbers[], size_t size);

/*
 * Description: A functoin that returns an array sorted in ascending order
 * Expected Input: array of numbers (int), length of array (size_t).
 * Expected Output: Returns an array sorted in ascending order
 * Return: SUCCESS, or ERROR if n is out of bounds.
 */
int bubbleSort(int numbers[], size_t size);

 #endif