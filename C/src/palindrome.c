#include "../inc/palindrome.h"

int isPalindrome(int num)
{
    int original_num = num;
    int reversed_num = 0;
    int remainder;

    if (0 > num)
    {
        return 0;
    }

    while (0 < num)
    {
        remainder = num % 10;

        reversed_num = (reversed_num * 10) + remainder;

        num /= 10;
    }

    if (original_num == reversed_num)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}