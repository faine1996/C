#include <cstring>
#include "../inc/bandana.h"

Bandana::Bandana(const char* newColor, int newLength)
{
    strncpy(color, newColor, BANDANA_COLOR_MAX_LEN - 1);
    color[BANDANA_COLOR_MAX_LEN - 1] = '\0';
    length = newLength;
}

Bandana::Bandana(const Bandana& other)
{
    strcpy(color, other.color);
    length = other.length;
}