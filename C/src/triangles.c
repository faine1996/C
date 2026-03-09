#include <stdio.h>
#include "../inc/triangles.h"

static int check_parameters(int n, char c);
static void print_row(int spaces, int chars, char c);

int func1(int n, char c)
{
    int row = 1;
    int status = check_parameters(n, c);

    if (SUCCESS != status)
    {
        return status;
    }

    for (; row <= n; ++row)
    {
        print_row(0, row, c);
    }

    return SUCCESS;
}

int func2(int n, char c)
{
    int row = n;
    int status = check_parameters(n, c);

    if (SUCCESS != status)
    {
        return status;
    }

    for (; row >= 1; --row)
    {
        print_row(0, row, c);
    }

    return SUCCESS;
}

int func3(int n, char c)
{
    int status = check_parameters(n, c);

    if (SUCCESS != status)
    {
        return status;
    }

    func1(n, c);
    func2(n - 1, c);

    return SUCCESS;
}

int func4(int n, char c)
{
    int row = 1;
    int status = check_parameters(n, c);

    if (SUCCESS != status)
    {
        return status;
    }

    for (; row <= n; ++row)
    {
        print_row(n - row, row, c);
    }

    return SUCCESS;
}

int func5(int n, char c)
{
    int row = n;
    int status = check_parameters(n, c);

    if (SUCCESS != status)
    {
        return status;
    }

    for (; row >= 1; --row)
    {
        print_row(n - row, row, c);
    }

    return SUCCESS;
}

int func6(int n, char c)
{
    int row = n - 1;
    int status = check_parameters(n, c);

    if (SUCCESS != status)
    {
        return status;
    }

    func4(n, c);

    for (; row >= 1; --row)
    {
        print_row(n - row, row, c);
    }

    return SUCCESS;
}

static int check_parameters(int n, char c)
{
    if (0 >= n || MAX_TRI_ROWS < n)
    {
        return ERROR;
    }
    
    if ('!' > c || '~' < c)
    {
        return ERROR;
    }

    return SUCCESS;
}

static void print_row(int spaces, int chars, char c)
{
    int space_idx = 1;
    int char_idx = 1;

    for (; space_idx <= spaces; ++space_idx)
    {
        putchar(' ');
    }

    for (; char_idx <= chars; ++char_idx)
    {
        putchar(c);
    }
    
    putchar('\n');
}