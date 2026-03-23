#ifndef SORT_LIB_H
#define SORT_LIB_H

typedef int (*s_criteria)(int, int);

/*int Sort(int *arr, int size, s_criteria CR);*/
int BubbleSort(int *arr, int size);

int Small2Big(int first, int second);
int Big2Small(int first, int second);
int EvensFirst(int first, int second);
int OddsFirst(int first, int second);

#define BUBBLE_SORT_MACRO(arr, size) \
    do \
    { \
        int i_m = 0; \
        int j_m = 0; \
        int temp_m = 0; \
        if (NULL != (arr) && 2 <= (size)) \
        { \
            for (i_m = 0; (size) - 1 > i_m; ++i_m) \
            { \
                for (j_m = 0; (size) - i_m - 1 > j_m; ++j_m) \
                { \
                    if ((arr)[j_m] > (arr)[j_m + 1]) \
                    { \
                        temp_m = (arr)[j_m]; \
                        (arr)[j_m] = (arr)[j_m + 1]; \
                        (arr)[j_m + 1] = temp_m; \
                    } \
                } \
            } \
        } \
    } while (0)

#endif