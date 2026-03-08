#include "../inc/reverse_num.h"

int is_reverse_num(int num)
{
    int reversed = 0;
    int is_negative = 0;
    int remainder = 0;

    if (0 == num)
    {
        return 0;
    }

    if (0 > num)
    {
        is_negative = 1;
        num *= -1;
    }

    while (0 < num)
    {
        remainder = num % 10;
        reversed = (reversed * 10) + remainder;
        num /= 10;
    }

    if (1 == is_negative)
    {
        reversed *= -1;
    }

    return reversed;
    
}