#ifndef STRING_FUNCS_H
#define STRING_FUNCS_H

#define ERROR -1
#define SUCCESS 0

/*
 * Description: A functoin that reverses an array of characters in place
 * Expected Input: An array of characters
 * Expected Output: Returns integer indicating success or failure
 * Return: SUCCESS, or ERROR if n is out of bounds.
 */

int ReverseStr(char *_str);

/*
 * Description: A functoin that checks if an array of characters are a palindrome
 * Expected Input: An array of characters
 * Expected Output: Returns integer indicating if the input is a palindrome or not
 * Return: SUCCESS, or ERROR if n is out of bounds.
 */

int IsPalindrome(char *_str);

#endif