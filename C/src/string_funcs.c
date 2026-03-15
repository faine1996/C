#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../inc/string_funcs.h"

int inputCheck(const char *_str);
void swapAndLower(char *a, char *b);
int outputCheck(int *result_ptr);

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

int MyAToI(char *_str, int *_num)
{

	const char* runner_ptr = NULL;
	int sign = 1;
	int result = 0;
    int status = 0;
    int inp_len;

    status = inputCheck(_str);
    if (status) 
    {
        return status;
    }

    status = outputCheck(_num);
    if (status) 
    {
        return status;
    }

    inp_len = strlen(_str);
    if (0 == inp_len) 
    {
        return ERROR; 
    }
	
	runner_ptr = _str;
	
	while (isspace(*runner_ptr))
	{
		++runner_ptr;
	}
	
	if ('-' == *runner_ptr)
	{
		sign = -1;
		++runner_ptr;
	}
	else if('+' == *runner_ptr)
	{
		++runner_ptr;
	}
	
	while ('0' <= *runner_ptr && '9' >= *runner_ptr)
	{
		result = (result * 10) + (*runner_ptr - '0');
		++runner_ptr;
	}
    *_num = result * sign;
	
	return SUCCESS;
}

int MyIToA(int _num, char *_buffer)
{
	char* runner_ptr = NULL;
	size_t is_neg = FALSE;
	size_t temp = 0;
    int status = 0;
	
	status = inputCheck(_buffer);
    if (status) 
    {
        return status;
    }
	
	runner_ptr = _buffer;
	
	if (0 == _num)
	{
		*runner_ptr = '0';
		++runner_ptr;
		*runner_ptr = '\0';
		
		return SUCCESS; 
	}
	
	if (0 > _num)
	{
		temp = -_num;
		is_neg = TRUE;
	}
	else
	{
		temp = _num;
	}
	
	while (temp)
	{
		*runner_ptr = ((temp % 10) + '0');
		++runner_ptr;
		temp /= 10;
	}
	
	if (is_neg)
	{
		*runner_ptr = '-';
		++runner_ptr;
		*runner_ptr = '\0';
	}
	
	*runner_ptr = '\0';
	
	ReverseStr(_buffer);
	
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

int outputCheck(int *result_ptr)
{
    if (NULL == result_ptr)
    {
        return ERROR;
    }
    return SUCCESS;
}