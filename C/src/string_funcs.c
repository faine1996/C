#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../inc/string_funcs.h"

int inputCheck(const char *_str);
void swapAndLower(char *a, char *b);

int ReverseStr(char *_str)
{
    int status = 0;
    int inp_len;
    char *begin_ptr;
    char *end_ptr;

    status = inputCheck(_str);
    if (status) 
    {
        return status;
    }

    inp_len = strlen(_str);
    if (0 == inp_len) 
    {
        return SUCCESS; 
    }

    begin_ptr = _str;
    end_ptr = _str + inp_len -1;
    
    while (begin_ptr < end_ptr)
    {
        swapAndLower(begin_ptr, end_ptr);
        
        ++begin_ptr;
        --end_ptr;
    }

    if (begin_ptr == end_ptr) 
    {
        *begin_ptr = tolower(*begin_ptr);
    }

    return SUCCESS;
}

int IsPalindrome(char *_str)
{
    int status = 0;
    int inp_len;
    char *begin_ptr;
    char *end_ptr;

    status = inputCheck(_str);
    if (status) 
    {
        return status;
    }

    inp_len = strlen(_str);
    if (0 == inp_len) 
    {
        return ERROR; 
    }

    begin_ptr = _str;
    end_ptr = _str + inp_len - 1;
    if (end_ptr < begin_ptr)
    {
        return SUCCESS;
    }

    while (begin_ptr < end_ptr)
    {
        if (*begin_ptr != *end_ptr)
        {
            return ERROR;
        }
        ++begin_ptr;
        --end_ptr;
    }
    return SUCCESS;

}

int inputCheck(const char *_str)
{
    if (NULL == _str)
    {
        return ERROR;
    }
    return SUCCESS;
}

void swapAndLower(char *a, char *b)
{
    char temp = tolower(*a);
    *a = tolower(*b);
    *b = temp;
}