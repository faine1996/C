#include <stddef.h>
#include "../inc/sort_lib.h"

static void Swap(int *a, int *b);
static int IsValidArray(int *arr, int size);

int Sort(int *arr, int size, s_criteria CR)
{
    int i = 0;
    int j = 0;

    if (0 == IsValidArray(arr, size) || NULL == CR)
    {
        return -1; 
    }

    for (i = 0; size - 1 > i; ++i)
    {
        for (j = 0; size - i - 1 > j; ++j)
        {
            if (CR(arr[j], arr[j + 1]))
            {
                Swap(&arr[j], &arr[j + 1]);
            }
        }
    }
    
    return 0;
}

int BubbleSort(int *arr, int size)
{
    int i = 0;
    int j = 0;

    if (0 == IsValidArray(arr, size))
    {
        return -1; 
    }

    for (i = 0; size - 1 > i; ++i)
    {
        for (j = 0; size - i - 1 > j; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                Swap(&arr[j], &arr[j + 1]);
            }
        }
    }
    
    return 0;
}

int Small2Big(int first, int second)
{
    return (first > second);
}

int Big2Small(int first, int second)
{
    return (first < second);
}

int EvensFirst(int first, int second)
{
    if ((0 != first % 2) && (0 == second % 2))
    {
        return 1;
    }
    if ((first % 2) == (second % 2))
    {
        return (first > second);
    }
    return 0;
}

int OddsFirst(int first, int second)
{
    if ((0 == first % 2) && (0 != second % 2))
    {
        return 1;
    }
    if ((first % 2) == (second % 2))
    {
        return (first > second);
    }
    return 0;
}

static void Swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

static int IsValidArray(int *arr, int size)
{
    if (NULL == arr || 2 > size)
    {
        return 0; 
    }
    return 1; 
}