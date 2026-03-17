#include <stdio.h>
#include <string.h>
#include "../inc/person_sort.h"

int SortPersonsById(struct Person *array, int size)
{
    int index;
    int j;
    struct Person temp;

    if (array == NULL || size <= 0)
    {
        return ERROR;
    }

    for (index = 0; index < size - 1; ++index)
    {
        for (j = 0; j < size - index - 1; ++j)
        {
            if (array[j].id > array[j + 1].id)
            {
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
    return SUCCESS;
}

int SortPersonsByName(struct Person *array, int size)
{
    int index;
    int j;
    struct Person temp;

    if (array == NULL || size <= 0)
    {
        return ERROR;
    }

    for (index = 0; index < size - 1; ++index)
    {
        for (j = 0; j < size - index - 1; ++j)
        {
            if (strcmp(array[j].name, array[j + 1].name) > 0)
            {
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
    return SUCCESS;
}