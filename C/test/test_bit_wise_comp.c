#include <stdio.h>
#include <string.h>
#include "../inc/bit_wise_comp.h"

void TestBitwiseImplementation(const char *test_str);
void TestBitFieldImplementation(const char *test_str);

int main(void)
{
    /* Using valid characters exclusively from 'a' to 'o' */
    const char *test_str_1 = "hello";
    const char *test_str_2 = "badcafe"; 
    
    printf("=== Testing Bitwise Implementation ===\n");
    TestBitwiseImplementation(test_str_1);
    TestBitwiseImplementation(test_str_2);
    
    printf("=== Testing Bit-Field Implementation ===\n");
    TestBitFieldImplementation(test_str_1);
    TestBitFieldImplementation(test_str_2);
    
    return 0;
}

void TestBitwiseImplementation(const char *test_str)
{
    unsigned char compressed_buffer[50] = {0};
    char decompressed_buffer[50] = {0};
    int bytes_written = 0;
    int i = 0;
    
    printf("Original String: %s\n", test_str);
    
    bytes_written = CompressBitwise(test_str, compressed_buffer);
    
    printf("Compressed (Hex): ");
    for (i = 0; bytes_written > i; ++i)
    {
        printf("%02X ", compressed_buffer[i]);
    }
    printf("\n");
    
    DecompressBitwise(compressed_buffer, bytes_written, decompressed_buffer);
    printf("Decompressed String: %s\n\n", decompressed_buffer);
}

void TestBitFieldImplementation(const char *test_str)
{
    struct BitFieldPair compressed_buffer[50] = {0};
    char decompressed_buffer[50] = {0};
    int pairs_written = 0;
    int i = 0;
    
    printf("Original String: %s\n", test_str);
    
    pairs_written = CompressBitField(test_str, compressed_buffer);
    
    printf("Compressed Bit-Fields (first:second): ");
    for (i = 0; pairs_written > i; ++i)
    {
        printf("%X:%X ", compressed_buffer[i].first, compressed_buffer[i].second);
    }
    printf("\n");
    
    DecompressBitField(compressed_buffer, pairs_written, decompressed_buffer);
    printf("Decompressed String: %s\n\n", decompressed_buffer);
}