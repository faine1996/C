#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../inc/string_funcs.h"

int checkInput(const void *ptr);
const char* skipWhiteSpace(const char *str);
const char* checkSign(const char *str, int *sign);
char* findWord(char *str, int *word_len);
void swapAndLower(char *a, char *b);
int reverseSegment(char* start_ptr, char* end_ptr);

int ReverseStr(char *_str)
{
    int status = 0;
    int inp_len;
    char *begin_ptr;
    char *end_ptr;

    status = checkInput(_str);
    if (0 != status) 
    {
        return status;
    }

    inp_len = strlen(_str);
    if (0 == inp_len) 
    {
        return SUCCESS; 
    }

    begin_ptr = _str;
    end_ptr = _str + inp_len - 1;
    
    while (begin_ptr < end_ptr)
    {
        swapAndLower(begin_ptr, end_ptr);
        
        ++begin_ptr;
        --end_ptr;
    }

    if (begin_ptr == end_ptr) 
    {
        *begin_ptr = tolower((unsigned char)*begin_ptr);
    }

    return SUCCESS;
}

int IsPalindrome(char *_str)
{
    int status = 0;
    int inp_len;
    char *begin_ptr;
    char *end_ptr;

    status = checkInput(_str);
    if (0 != status) 
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

    status = checkInput(_str);
    if (0 != status) 
    {
        return status;
    }

    status = checkInput(_num);
    if (0 != status) 
    {
        return status;
    }
    
    runner_ptr = skipWhiteSpace(_str);
    runner_ptr = checkSign(runner_ptr, &sign);
    
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
    int is_neg = FALSE;
    int status = 0;
    
    status = checkInput(_buffer);
    if (0 != status) 
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
        _num = -_num; 
        is_neg = TRUE;
    }
    
    while (0 != _num)
    {
        *runner_ptr = ((_num % 10) + '0');
        ++runner_ptr;
        _num /= 10;
    }
    
    if (TRUE == is_neg)
    {
        *runner_ptr = '-';
        ++runner_ptr;
    }
    
    *runner_ptr = '\0';
    ReverseStr(_buffer);
    
    return SUCCESS;
}

int ReverseWordsInString(char* _str)
{
    int status = 0;
    char* runner = NULL;
    char* start_word = NULL;
    int inp_len = 0;
    int word_len = 0;

    status = checkInput(_str);
    if (0 != status) 
    {
        return status;
    }

    inp_len = strlen(_str);
    if (0 == inp_len) 
    {
        return SUCCESS; 
    }

    reverseSegment(_str, _str + inp_len - 1);
    runner = _str;

    while (NULL != (start_word = findWord(runner, &word_len)))
    {
        reverseSegment(start_word, start_word + word_len - 1);
        runner = start_word + word_len;
    }

    return SUCCESS;
}

int countWordsInString(char* _str)
{
    int status = 0;
    int word_count = 0;
    char* runner = NULL;
    char* start_word = NULL;
    int word_len = 0;

    status = checkInput(_str);
    if (0 != status) 
    {
        return ERROR;
    }

    runner = _str;

    while (NULL != (start_word = findWord(runner, &word_len)))
    {
        ++word_count;
        runner = start_word + word_len; 
    }

    return word_count;
}

int checkInput(const void *ptr)
{
    if (NULL == ptr)
    {
        return ERROR;
    }
    return SUCCESS;
}

const char* skipWhiteSpace(const char *str)
{
    while (0 != isspace((unsigned char)*str))
    {
        ++str;
    }
    return str;
}

const char* checkSign(const char *str, int *sign)
{
    *sign = 1;
    
    if ('-' == *str)
    {
        *sign = -1;
        ++str;
    }
    else if ('+' == *str)
    {
        ++str;
    }
    
    return str;
}

char* findWord(char *str, int *word_len)
{
    char *start_word;
    *word_len = 0;
    
    str = (char*)skipWhiteSpace(str); 
    
    if ('\0' == *str)
    {
        return NULL; 
    }
    
    start_word = str;
    
    while ('\0' != *str && 0 == isspace((unsigned char)*str))
    {
        (*word_len)++;
        ++str;
    }
    
    return start_word;
}

void swapAndLower(char *a, char *b)
{
    char temp;

    temp = tolower((unsigned char)*a);
    *a = tolower((unsigned char)*b);
    *b = temp;
}

int reverseSegment(char* start_ptr, char* end_ptr)
{
    char temp_char;

    while (start_ptr < end_ptr)
    {
        temp_char = *start_ptr;
        *start_ptr = *end_ptr;
        *end_ptr = temp_char;

        ++start_ptr;
        --end_ptr;
    }

    return SUCCESS;
}