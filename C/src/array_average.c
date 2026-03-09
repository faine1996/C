#include <stdio.h>

#include "../inc/array_average.h"

float arrayAverage(int array_avg[], size_t size)
{
    int sum = 0;
    size_t index = 0;

    if (0 == size)
    {
        return 0.0;
    }

    for (; index < size; ++index)
    {
        sum += array_avg[index];
    }

    return (float)sum / size;

}