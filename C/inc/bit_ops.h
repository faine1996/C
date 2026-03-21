#ifndef BIT_OPS_H
#define BIT_OPS_H

#define BITS_IN_BYTE 8

unsigned char InvertBits(unsigned char x);
unsigned char RotateRight(unsigned char x);
unsigned char SetBits(unsigned char x, int p, int n, unsigned char y);
unsigned int ReverseBits(unsigned int x);

void PrintBinary(unsigned char val);
void PrintBinaryInt(unsigned int val);

#endif