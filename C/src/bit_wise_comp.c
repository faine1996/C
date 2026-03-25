#include <stddef.h>
#include "../inc/bit_wise_comp.h"

static unsigned char CharToNibble(char c);
static char NibbleToChar(unsigned char n);

static unsigned char CharToNibble(char c)
{
    if ('\0' == c)
    {
        return 0;
    }
    if (c >= 'a' && c <= 'o')
    {
        return (unsigned char)(c - 'a' + 1);
    }
    return 0; /* Fallback for invalid characters */
}

static char NibbleToChar(unsigned char n)
{
    if (0 == n)
    {
        return '\0';
    }
    if (n >= 1 && n <= 15)
    {
        return (char)(n - 1 + 'a');
    }
    return '\0';
}

int CompressBitwise(const char *src, unsigned char *dest)
{
    int i = 0;
    int j = 0;
    unsigned char nibble1 = 0;
    unsigned char nibble2 = 0;

    if (NULL == src || NULL == dest)
    {
        return -1;
    }

    while ('\0' != src[i])
    {
        nibble1 = CharToNibble(src[i]);
        i++;
        
        if ('\0' != src[i])
        {
            nibble2 = CharToNibble(src[i]);
            i++;
        }
        else
        {
            nibble2 = 0; /* Pad the second half of the byte if string length is odd */
        }
        
        /* Shift first character to the upper 4 bits, combine with the second */
        dest[j] = (unsigned char)((nibble1 << 4) | nibble2);
        j++;
    }
    
    return j; 
}

void DecompressBitwise(const unsigned char *src, int compressed_bytes, char *dest)
{
    int i = 0;
    int j = 0;
    unsigned char nibble1 = 0;
    unsigned char nibble2 = 0;

    if (NULL == src || NULL == dest)
    {
        return;
    }

    for (i = 0; compressed_bytes > i; ++i)
    {
        /* Extract upper 4 bits */
        nibble1 = (unsigned char)((src[i] >> 4) & 0x0F);
        /* Extract lower 4 bits */
        nibble2 = (unsigned char)(src[i] & 0x0F);
        
        dest[j] = NibbleToChar(nibble1);
        j++;
        
        /* Only decode the second character if the first wasn't a null terminator */
        if ('\0' != dest[j - 1])
        {
            dest[j] = NibbleToChar(nibble2);
            j++;
        }
    }
    dest[j] = '\0'; 
}

int CompressBitField(const char *src, struct BitFieldPair *dest)
{
    int i = 0;
    int j = 0;
    unsigned char nibble1 = 0;
    unsigned char nibble2 = 0;

    if (NULL == src || NULL == dest)
    {
        return -1;
    }

    while ('\0' != src[i])
    {
        nibble1 = CharToNibble(src[i]);
        i++;
        
        if ('\0' != src[i])
        {
            nibble2 = CharToNibble(src[i]);
            i++;
        }
        else
        {
            nibble2 = 0; 
        }
        
        /* Assignment directly to the bit-fields handles the masking implicitly */
        dest[j].first = nibble1;
        dest[j].second = nibble2;
        j++;
    }
    
    return j;
}

void DecompressBitField(const struct BitFieldPair *src, int compressed_pairs, char *dest)
{
    int i = 0;
    int j = 0;

    if (NULL == src || NULL == dest)
    {
        return;
    }

    for (i = 0; compressed_pairs > i; ++i)
    {
        dest[j] = NibbleToChar((unsigned char)src[i].first);
        j++;
        
        if ('\0' != dest[j - 1])
        {
            dest[j] = NibbleToChar((unsigned char)src[i].second);
            j++;
        }
    }
    dest[j] = '\0';
}