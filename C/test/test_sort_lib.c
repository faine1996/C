#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "../inc/sort_lib.h"

int *GenerateRandomArray(int size, int max_val);
void PrintArraySample(int *arr, int size_to_print);
void RunCriteriaTests(int size, int max_val);
void RunBenchmarkTest(int size, int max_val);
double GetElapsedTime(struct timeval start, struct timeval end);

int main(void)
{
    int array_size = 60000;
    int max_value = 80000; 

    srand((unsigned int)time(NULL));

    RunCriteriaTests(10, max_value);

    RunBenchmarkTest(array_size, max_value);

    return 0;
}

void RunCriteriaTests(int size, int max_val)
{
    int *data = NULL;
    
    printf("=== Function Pointer Criteria Tests ===\n");
    
    data = GenerateRandomArray(size, max_val);
    Sort(data, size, Small2Big);
    printf("Small2Big:  ");
    PrintArraySample(data, size);
    free(data);

    data = GenerateRandomArray(size, max_val);
    Sort(data, size, Big2Small);
    printf("Big2Small:  ");
    PrintArraySample(data, size);
    free(data);

    data = GenerateRandomArray(size, max_val);
    Sort(data, size, EvensFirst);
    printf("EvensFirst: ");
    PrintArraySample(data, size);
    free(data);

    data = GenerateRandomArray(size, max_val);
    Sort(data, size, OddsFirst);
    printf("OddsFirst:  ");
    PrintArraySample(data, size);
    free(data);
    printf("\n");
}

void RunBenchmarkTest(int size, int max_val)
{
    size_t byte_size = size * sizeof(int);
    int *data_for_func = NULL;
    int *data_for_macro = NULL;
    
    struct timeval start_time;
    struct timeval end_time;
    double time_func = 0.0;
    double time_macro = 0.0;

    printf("=== Benchmark: Function vs Macro (%d elements) ===\n", size);
    
    data_for_func = GenerateRandomArray(size, max_val);
    data_for_macro = (int *)malloc(byte_size);

    if (NULL == data_for_func || NULL == data_for_macro)
    {
        printf("Memory allocation failed.\n");
        free(data_for_func);
        free(data_for_macro);
        return;
    }

    memcpy(data_for_macro, data_for_func, byte_size);

    printf("Sorting via BubbleSort Function... ");
    gettimeofday(&start_time, NULL);
    BubbleSort(data_for_func, size);
    gettimeofday(&end_time, NULL);
    
    time_func = GetElapsedTime(start_time, end_time);
    printf("Took %f seconds.\n", time_func);

    printf("Sorting via BUBBLE_SORT_MACRO...   ");
    gettimeofday(&start_time, NULL);
    BUBBLE_SORT_MACRO(data_for_macro, size);
    gettimeofday(&end_time, NULL);
    
    time_macro = GetElapsedTime(start_time, end_time);
    printf("Took %f seconds.\n", time_macro);

    free(data_for_func);
    free(data_for_macro);
}

int *GenerateRandomArray(int size, int max_val)
{
    int *arr = NULL;
    int i = 0;

    if (0 >= size || 0 >= max_val)
    {
        return NULL;
    }

    arr = (int *)malloc(size * sizeof(int));
    if (NULL == arr)
    {
        return NULL;
    }

    for (i = 0; size > i; ++i)
    {
        arr[i] = rand() % max_val;
    }

    return arr;
}

void PrintArraySample(int *arr, int size_to_print)
{
    int i = 0;
    for (i = 0; size_to_print > i; ++i)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

double GetElapsedTime(struct timeval start, struct timeval end)
{
    return (double)(end.tv_sec - start.tv_sec) + 
           (double)(end.tv_usec - start.tv_usec) / 1000000.0;
}