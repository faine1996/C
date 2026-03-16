#ifndef STRING_FUNCS_H
#define STRING_FUNCS_H

#define ERROR -1
#define SUCCESS 0
#define BUFFER_SIZE (100)

typedef enum {
    FALSE = 0,
    TRUE = 1 
} Flag;

/*
 * Description: A function that reverses an array of characters in place
 * Expected Input: An array of characters
 * Expected Output: Returns integer indicating success or failure
 * Return: SUCCESS, or ERROR if n is out of bounds.
 */

int ReverseStr(char *_str);

/*
 * Description: A function that checks if an array of characters are a palindrome
 * Expected Input: An array of characters
 * Expected Output: Returns integer indicating if the input is a palindrome or not
 * Return: SUCCESS, or ERROR if n is out of bounds.
 */

int IsPalindrome(char *_str);

/*
 * Description: A function that takes ascii characters and returns the integer equivalent
 * Expected Input: An array of characters
 * Expected Output: Returns integer indicating conversion was successful
 * Return: SUCCESS, or ERROR if n is out of bounds.
 */

int MyAToI(char *_str, int *_num);

/*
 * Description: A function that takes integer and returns the ascii equivalent
 * Expected Input: An integer
 * Expected Output: Returns integer indicating conversion was successful
 * Return: SUCCESS, or ERROR if n is out of bounds.
 */

int MyIToA(int _num, char *_buffer);

/*
 * Description: A function that takes a array of characters and reverses the order of words
 * Expected Input: An array of characters
 * Expected Output: Returns integer indicating reversal was successful
 * Return: SUCCESS, or ERROR if n is out of bounds.
 */

int ReverseWordsInString(char* _str);

/*
 * Description: A function that counts the number of words in a string of words
 * Expected Input: An array of characters
 * Expected Output: Returns integer indicating number of words
 * Return: Number of words counted or -1 for empty string.
 */

int countWordsInString(char* _str);

#endif