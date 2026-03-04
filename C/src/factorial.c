#include "../inc/factorial.h"

unsigned long calculateFactorial(int n) 
{
    unsigned long result = 1;
    int i = 1;

    if (n < 0) 
    {
        return 0; 
    }

    for (i; i <= n; i++) 
    {
        result *= i;
    }

    return result;
}