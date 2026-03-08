#include <stdio.h>

int mastermind(int base, int check)
{
    int exact_matches = 0;
    int total_shared = 0;
    int temp_base = 0;
    int temp_check = 0;
    int current_check_digit = 0;
    int current_base_digit = 0;
    int partial_matches = 0;

    if (0 > base || 0 > check)
    {
        return -1;
    }

    temp_base = base;
    temp_check = check;

    while (0 < temp_base && 0 < temp_check)
    {
        if ((temp_base % 10) == (temp_check % 10))
        {
            exact_matches += 1;
        }

        temp_base /= 10;
        temp_check /= 10;
    }

    temp_check = check;

    while (0 < temp_check)
    {
        current_check_digit  = temp_check % 10;
        temp_base = base;

        while (0 < temp_base)
        {
            current_base_digit = temp_base % 10;

            if (current_check_digit == current_base_digit)
            {
                total_shared += 1;
                break;
            }

            temp_base /= 10;
        }

        temp_check /= 10;
    }

    partial_matches = total_shared - exact_matches;

    return (exact_matches * 10) + partial_matches;
}