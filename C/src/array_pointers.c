#include <stdio.h>
#include <stdlib.h>

#include "../inc/array_pointers.h"

int inputCheck(int *numbers, size_t size);
int outputCheck(int *result_ptr);
int swapInts(int *ptr1, int *ptr2);
int shiftRight(int *start_ptr, int *end_ptr);

int maxNumberApp(int *numbers, size_t size, int *result)
{
    int status;
    size_t max_app;
    size_t current_app;
    int *current_val_ptr;
    int *scan_ptr;
    int *arr_end;

    max_app = 0;

    status = inputCheck(numbers, size);
    if (SUCCESS != status) return status;
    
    status = outputCheck(result);
    if (SUCCESS != status) return status;

    arr_end = numbers + size;
    *result = *numbers;

    for (current_val_ptr = numbers; current_val_ptr < arr_end; ++current_val_ptr)
    {
        current_app = 1;
        for (scan_ptr = current_val_ptr + 1; scan_ptr < arr_end; ++scan_ptr)
        {
            if (*current_val_ptr == *scan_ptr)
            {
                ++current_app;
            }
        }
        
        if (current_app > max_app)
        {
            max_app = current_app;
            *result = *current_val_ptr;
        }
    }
    return SUCCESS;
}

int organizeEvenOdd(int *numbers, size_t size, int *even_count)
{
    int status;
    int *eval_ptr;
    int *next_even_ptr;
    int *arr_end;
    int temp_even;
    int count;

    status = inputCheck(numbers, size);
    if (SUCCESS != status) return status;
    
    status = outputCheck(even_count);
    if (SUCCESS != status) return status;

    arr_end = numbers + size;
    eval_ptr = numbers;
    count = 0;

    while (eval_ptr < arr_end)
    {
        if (0 != *eval_ptr % 2) 
        {
            /* find the next even number */
            next_even_ptr = eval_ptr + 1;
            while (next_even_ptr < arr_end && 0 != *next_even_ptr % 2) 
            {
                ++next_even_ptr;
            }

            /* break if no more evens exist ahead */
            if (next_even_ptr == arr_end)
            {
                break; 
            }

            temp_even = *next_even_ptr;
            
            /* utilize the helper function to handle the memory shift */
            status = shiftRight(eval_ptr, next_even_ptr);
            if (SUCCESS != status) return status;

            *eval_ptr = temp_even;
            ++count;
        }
        else
        {
            ++count;
        }
        
        ++eval_ptr;
    }

    *even_count = count;
    return SUCCESS;
}

int bubbleSort(int *numbers, size_t size)
{
    int status;
    int *pass_end_ptr;
    int *compare_ptr;
    int swap_flag;

    status = inputCheck(numbers, size);
    if (SUCCESS != status) return status;

    for (pass_end_ptr = numbers + size; pass_end_ptr > numbers; --pass_end_ptr)
    {
        swap_flag = 0; 

        for (compare_ptr = numbers + 1; compare_ptr < pass_end_ptr; ++compare_ptr)
        {
            if (*compare_ptr < *(compare_ptr - 1))
            {
                /* utilize the helper function for a cleaner swap */
                status = swapInts(compare_ptr - 1, compare_ptr);
                if (SUCCESS != status) return status;
                
                swap_flag = 1;
            }
        }

        /* break early if array is already sorted */
        if (0 == swap_flag) 
        {
            break; 
        }
    }
    
    return SUCCESS;
}

int sortBinaryArray(int *numbers, size_t size)
{
    int status;
    int *scan_ptr;
    int *next_zero_ptr;
    int *arr_end;

    status = inputCheck(numbers, size);
    if (SUCCESS != status) return status;

    arr_end = numbers + size;
    
    /* tracks where the next '0' should be placed */
    next_zero_ptr = numbers; 

    for (scan_ptr = numbers; scan_ptr < arr_end; ++scan_ptr)
    {
        if (0 == *scan_ptr) 
        {
            /* throw the found 0 to the front of the array */
            status = swapInts(scan_ptr, next_zero_ptr);
            if (SUCCESS != status) return status;
            
            ++next_zero_ptr;
        }
    }

    return SUCCESS;
}

/* validates the input array */
int inputCheck(int *numbers, size_t size)
{
    if (NULL == numbers || 0 == size)
    {
        return ERROR;
    }
    return SUCCESS;
}

/* validates the output pointer where results are stored */
int outputCheck(int *result_ptr)
{
    if (NULL == result_ptr)
    {
        return ERROR;
    }
    return SUCCESS;
}

/* swaps the values of two integer pointers */
int swapInts(int *ptr1, int *ptr2)
{
    int temp;

    if (NULL == ptr1 || NULL == ptr2)
    {
        return ERROR;
    }

    temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;

    return SUCCESS;
}

/* shifts a block of memory one position to the right */
int shiftRight(int *start_ptr, int *end_ptr)
{
    int *shift_ptr;

    if (NULL == start_ptr || NULL == end_ptr || start_ptr > end_ptr)
    {
        return ERROR;
    }

    shift_ptr = end_ptr;
    while (shift_ptr > start_ptr)
    {
        *shift_ptr = *(shift_ptr - 1);
        --shift_ptr;
    }

    return SUCCESS;
}