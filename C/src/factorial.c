#include "../inc/factorial.h"

unsigned long long calculateFactorial(int n) 
{
    unsigned long long result;
    int i;

    if (n < 0) 
    {
        return 0; 
    }

    result = 1;
    i = 1;
    for (; i <= n; ++i) 
    {
        result *= i;
    }

    return result;
}