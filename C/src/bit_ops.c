#include <stdio.h>
#include "../inc/bit_ops.h"

unsigned char InvertBits(unsigned char x)
{
    unsigned char y;

    y = (unsigned char)(~x);

    return y;
}

unsigned char RotateRight(unsigned char x)
{
    unsigned char dropped_bit;
    unsigned char shifted_x;
    unsigned char result;

    dropped_bit = x & 1;
    shifted_x = x >> 1;
    result = shifted_x | (unsigned char)(dropped_bit << 7);

    return result;
}

unsigned char SetBits(unsigned char x, int p, int n, unsigned char y)
{
    int shift;
    unsigned char n_ones;
    unsigned char isolated_y;
    unsigned char hole_mask;
    unsigned char carved_x;
    unsigned char aligned_y;
    unsigned char result;

    shift = p - n;
    n_ones = (unsigned char)((1 << n) - 1);
    isolated_y = y & n_ones;
    hole_mask = (unsigned char)(~(n_ones << shift));
    carved_x = x & hole_mask;
    aligned_y = (unsigned char)(isolated_y << shift);
    result = carved_x | aligned_y;

    return result;
}

unsigned int ReverseBits(unsigned int x)
{
    unsigned int result;
    int num_bits;
    int i;

    result = 0;
    num_bits = sizeof(x) * BITS_IN_BYTE;

    for (i = 0; i < num_bits; i++)
    {
        result = (result << 1) | (x & 1);
        x >>= 1;
    }

    return result;
}

void PrintBinary(unsigned char val)
{
    int i;
    
    for (i = 7; i >= 0; i--)
    {
        printf("%d", (val >> i) & 1);
    }
}

void PrintBinaryInt(unsigned int val)
{
    int i;
    int num_bits;

    num_bits = sizeof(val) * BITS_IN_BYTE;
    
    for (i = num_bits - 1; i >= 0; i--)
    {
        printf("%d", (val >> i) & 1);
    }
}