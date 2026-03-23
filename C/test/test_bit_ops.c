#include <stdio.h>
#include "../inc/bit_ops.h"

void testInvertBitsNormal(void);
void testRotateRightNormal(void);
void testSetBitsNormal(void);
void testReverseBitsNormal(void);
void testReverseBitsLUTNormal(void);

int main(void)
{
    printf("--- Running Bit Operations Tests ---\n\n");

    testInvertBitsNormal();
    testRotateRightNormal();
    testSetBitsNormal();
    testReverseBitsNormal();
    testReverseBitsLUTNormal();

    printf("\n--- All Tests Complete ---\n");
    return 0;
}

void testInvertBitsNormal(void)
{
    unsigned char input;
    unsigned char expected;
    unsigned char result;

    input = 0xEA;
    expected = 0x15;

    printf("Test InvertBits 1: Inverting 0xEA (11101010)\n");
    result = InvertBits(input);
    
    printf("Expected: 0x%02X, Binary: ", expected);
    PrintBinary(expected);
    printf("\n");

    printf("Got     : 0x%02X, Binary: ", result);
    PrintBinary(result);
    printf("\n\n");
}

void testRotateRightNormal(void)
{
    unsigned char input;
    unsigned char expected;
    unsigned char result;

    input = 0xEB;
    expected = 0xF5;

    printf("Test RotateRight 1: Rotating 0xEB (11101011) right by 1\n");
    result = RotateRight(input);

    printf("Expected: 0x%02X, Binary: ", expected);
    PrintBinary(expected);
    printf("\n");

    printf("Got     : 0x%02X, Binary: ", result);
    PrintBinary(result);
    printf("\n\n");
}

void testSetBitsNormal(void)
{
    unsigned char x;
    unsigned char y;
    int p;
    int n;
    unsigned char expected;
    unsigned char result;

    x = 170;
    y = 167;
    p = 6;
    n = 3;
    expected = 186;

    printf("Test SetBits 1: x=170, y=167, p=6, n=3\n");
    result = SetBits(x, p, n, y);

    printf("Expected: %d, Binary: ", expected);
    PrintBinary(expected);
    printf("\n");

    printf("Got     : %d, Binary: ", result);
    PrintBinary(result);
    printf("\n\n");
}

void testReverseBitsNormal(void)
{
    unsigned int input;
    unsigned int expected;
    unsigned int result;

    input = 0xF0F0F0F0;
    expected = 0x0F0F0F0F;

    printf("Test ReverseBits 1: Reversing 0xF0F0F0F0\n");
    result = ReverseBits(input);

    printf("Expected: 0x%08X, Binary: ", expected);
    PrintBinaryInt(expected);
    printf("\n");

    printf("Got     : 0x%08X, Binary: ", result);
    PrintBinaryInt(result);
    printf("\n\n");
}

void testReverseBitsLUTNormal(void)
{
    unsigned int input;
    unsigned int expected;
    unsigned int result;

    input = 0x12345678;
    expected = 0x1E6A2C48;

    printf("Test ReverseBitsLUT 1: Reversing 0x12345678 using Dynamic LUT\n");
    result = ReverseBitsLUT(input);

    printf("Expected: 0x%08X, Binary: ", expected);
    PrintBinaryInt(expected);
    printf("\n");

    printf("Got     : 0x%08X, Binary: ", result);
    PrintBinaryInt(result);
    printf("\n\n");
}