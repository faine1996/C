#include "../inc/ascending.h"

int isAscending(int num)
{
    int prev_digit = 0;
    int curr_digit = 0;

    if (num < 0)
    {
        num *= -1;
    }

    prev_digit = num % 10;
    num /= 10;

    while (0 < num)
    {
        curr_digit = num % 10;

        if (curr_digit > prev_digit)
        {
            return 0;
        }

        prev_digit = curr_digit;
        num /= 10;
    }

    return 1;

}