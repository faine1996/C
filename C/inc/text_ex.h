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

#endif