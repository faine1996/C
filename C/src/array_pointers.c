#include <stdio.h>

#include "../inc/array_pointers.h"

int maxNumberApp(int numbers[], size_t size)
{
    size_t max_app = 0;
    int current_num = 0;
    int current_max_num = 0;
    size_t max_number_app = 0;
    size_t i = 0;
    size_t j = 0;

    if (0 == size || !numbers)
    {
        return ERROR;
    }

    current_num = numbers[0];
    for (; i < size; ++i)
    {
        current_num = numbers[i];
        j = 0;
        max_number_app = 1;
        for (j = i + 1; j < size; ++j)
        {
            if (current_num == numbers[j])
            {
                max_number_app += 1;
            }
        }
        if (max_number_app > max_app)
        {
            max_app = max_number_app;
            current_max_num = current_num;
        }
    }
    return current_max_num;
}