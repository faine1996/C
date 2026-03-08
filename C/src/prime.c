#include "../inc/prime.h"


int isPrime(int num)
{
    int index;

    if (1 >= num)
    {
        return 0;
    }
    if (2 == num)
    {
        return 1;
    }
    if (0 == num % 2)
    {
        return 0;
    }

    while (index < num / index)
    {
        if (0 == num % index)
        {
            return 0;
        }
        index += 2;
    }

    return 1;

}