#ifndef _TEXT_EX_
#define _TEXT_EX_

#include <stdio.h>

/*
 * Reads an open text file and prints the last 'n' lines to standard output.
 * Uses a dynamic circular buffer to safely and efficiently read the file 
 * sequentially.
 *
 * in: A pointer to an open FILE stream (must be open for reading).
 * n:  The number of lines from the end of the file to print.
 */
void tail(FILE* in, int n);

/*
 * wc: Computes the number of lines, words, and characters in a file.
 *
 * fp:    Pointer to the open file stream.
 * lines: Output parameter for the line count.
 * words: Output parameter for the word count.
 * chars: Output parameter for the byte/character count.
 *
 * A word is defined as a maximal string of characters delimited by whitespace
 * (spaces, tabs, newlines) as evaluated by the standard isspace() function.
 * Output pointers are null-checked, allowing callers to pass NULL if a 
 * specific count is not needed.
 */
void wc(FILE* fp, unsigned long* lines, unsigned long* words, unsigned long* chars);

#endif