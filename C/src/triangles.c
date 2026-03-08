#include <stdio.h>

void func1(int n, char c)
{
    int row = 1;
    int col = 1;

    for (; row <= n; ++row)
    {
        for (col = 1; col <= row; ++col)
        {
            putchar(c);
        }
        putchar('\n');
    }
}

void func2(int n, char c)
{
    int row = n;
    int col = 1;

    for (; row >= 1; --row)
    {
        for (col = 1; col <= row; ++col)
        {
            putchar(c);
        }
        putchar('\n');
    }
}

void func3(int n, char c)
{
    func1(n, c);
    func2(n - 1, c);
}

void func4(int n, char c)
{
    int row = 1;
    int space = 1;
    int col = 1;

    for (; row <= n; ++row)
    {
        for (space = 1; space <= n - row; ++space)
        {
            putchar(' ');
        }

        for (col = 1; col <= row; ++col)
        {
            putchar(c);
        }
        putchar('\n');
    }
}

void func5(int n, char c)
{
    int row = n;
    int space = 1;
    int col = 1;

    for (; row >= 1; --row)
    {
        for (space = 1; space <= n - row; ++space)
        {
            putchar(' ');
        }

        for (col = 1; col <= row; ++col)
        {
            putchar(c);
        }
        putchar('\n');
    }
}

void func6(int n, char c)
{
    func4(n, c);
    func5(n - 1, c);
}