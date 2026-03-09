#ifndef TRIANGLES_H
#define TRIANGLES_H

/* Status Codes */
#define SUCCESS 0
#define ERROR -1

/* System Limits */
#define MAX_TRI_ROWS 80

/*
 * Description: Outputs a left-aligned ascending triangle.
 * Expected Input: n (lines, 1 to MAX_TRI_ROWS), c (character).
 * Expected Output: Prints shape to standard output.
 * Return: SUCCESS, or ERROR if n is out of bounds.
 */
int func1(int n, char c);

/*
 * Description: Outputs a left-aligned descending triangle.
 * Expected Input: n (lines, 1 to MAX_TRI_ROWS), c (character).
 * Expected Output: Prints shape to standard output.
 * Return: SUCCESS, or ERROR if n is out of bounds.
 */
int func2(int n, char c);

/*
 * Description: Outputs a left-aligned arrow of (n * 2 - 1) lines.
 * Expected Input: n (half-arrow lines, 1 to MAX_TRI_ROWS), c (character).
 * Expected Output: Prints shape to standard output.
 * Return: SUCCESS, or ERROR if n is out of bounds.
 */
int func3(int n, char c);

/*
 * Description: Outputs a right-aligned ascending triangle.
 * Expected Input: n (lines, 1 to MAX_TRI_ROWS), c (character).
 * Expected Output: Prints shape to standard output.
 * Return: SUCCESS, or ERROR if n is out of bounds.
 */
int func4(int n, char c);

/*
 * Description: Outputs a right-aligned descending triangle.
 * Expected Input: n (lines, 1 to MAX_TRI_ROWS), c (character).
 * Expected Output: Prints shape to standard output.
 * Return: SUCCESS, or ERROR if n is out of bounds.
 */
int func5(int n, char c);

/*
 * Description: Outputs a right-aligned arrow of (n * 2 - 1) lines.
 * Expected Input: n (half-arrow lines, 1 to MAX_TRI_ROWS), c (character).
 * Expected Output: Prints shape to standard output.
 * Return: SUCCESS, or ERROR if n is out of bounds.
 */
int func6(int n, char c);

#endif /* TRIANGLES_H */