#ifndef BIT_OPS_H
#define BIT_OPS_H

#define BITS_IN_BYTE 8

/*
 * Description: Inverts the bits of an unsigned char.
 * Expected Input: unsigned character.
 * Expected Output: The bit result in binary form.
 * Return: Returns the inverted unsigned char.
 */

unsigned char InvertBits(unsigned char x);

/*
 * Description: A function that rotates the bits of an unsigned char without shifts.
 * Expected Input: Unsigned character.
 * Expected Output: The bit result in binary form.
 * Return: Returns the shifted unsigned char.
 */

unsigned char RotateRight(unsigned char x);

/*
 * Description: Take unsigned char x with the n bits that begin at position
p set to the rightmost n bits of an unsigned char variable y.
 * Expected Input: Unsigned char x, y and integers p, n.
 * Expected Output: The bit result in binary form.
 * Return: The unsigned char x modified by y bits.
 */

unsigned char SetBits(unsigned char x, int p, int n, unsigned char y);

/*
 * Description:A function that reverse the bits in an integer, so that the most significant bit and the
least significant bit are swapped.
 * Expected Input: Unsigned char x.
 * Expected Output: The bit result in binary form.
 * Return: The modified integer.
 */

unsigned int ReverseBits(unsigned int x);

void PrintBinary(unsigned char val);
void PrintBinaryInt(unsigned int val);

#endif