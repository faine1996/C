#include <stdio.h>
#include <stdlib.h>

#include "../inc/array_pointers.h"

int inputCheck(int numbers[], size_t size);

int maxNumberApp(int numbers[], size_t size)
{
    size_t max_app = 0;
    int current_num = 0;
    int current_max_num = 0;
    size_t max_number_app = 0;
    size_t i = 0;
    size_t j = 0;
    int status = 0;

    status = inputCheck(numbers,size);

    if (0 != status)
    {
        return status;
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
                ++max_number_app;
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

int organizeEvenOdd(int numbers[], size_t size)
{
    int *temp_arr = NULL;
    size_t even_count = 0;
    int status = 0;
    int* even_ptr = NULL;
    int* odd_ptr = NULL;
    int* runner = NULL;
    size_t i = 0;

    status = inputCheck(numbers,size);
    if (0 != status)
    {
        return status;
    }
    runner = numbers;

    temp_arr = malloc(size * sizeof(int));
    if (NULL == temp_arr)
    {
        return ERROR;
    }

    even_ptr = temp_arr;
    odd_ptr = temp_arr;
    
    while (runner < numbers + size)
    {
        if (0 == *runner % 2)
        {
            ++even_count;
        }
        ++runner;
    }

    even_ptr = temp_arr;
    odd_ptr = temp_arr + even_count;
    runner = numbers;
    while (runner < numbers + size)
    {
        if (0 == *runner % 2)
        {
            *even_ptr = *runner;
            ++even_ptr;
        }
        else
        {
            *odd_ptr = *runner;
            ++odd_ptr; 
        }
        ++runner;
    }

    for (; i < size; ++i) 
    {
        *(numbers + i) = *(temp_arr + i);
    }

    free(temp_arr);
    return even_count;
}

int bubbleSort(int numbers[], size_t size)
{
    int status = 0;
    size_t out_ind = 0;
    size_t in_ind = 1;
    int temp = 0;
    int swap_flag = 0;


    status = inputCheck(numbers,size);
    if (0 != status)
    {
        return status;
    }

    for (; out_ind < size; ++out_ind)
    {
        swap_flag = 0;
        for (in_ind = 1;in_ind < size - out_ind; ++in_ind)
        {
            if (numbers[in_ind] < numbers[in_ind - 1])
            {
                temp = numbers[in_ind -1];
                numbers[in_ind -1] = numbers[in_ind];
                numbers[in_ind] = temp;
                swap_flag = 1;
            }
        }

        if (0 == swap_flag) 
        {
            break; 
        }
    }
    return SUCCESS;
}

int inputCheck(int numbers[], size_t size)
{
    if (0 == size || !numbers)
    {
        return ERROR;
    }

    return SUCCESS;
}