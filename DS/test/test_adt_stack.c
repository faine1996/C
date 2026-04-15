#include <stdio.h>
#include "../inc/adt_stack.h"

/* --- Function Declarations --- */
int isMatchingPair(char _openChar, char _closeChar);
int areParenthesesBalanced(const char* _expression);

/* --- Main Function --- */
int main(void)
{
    const char* expr1 = "{x+(y-[a+b]*c-[(d+e)])/(h-(j-(k-[l-n])))}";
    const char* expr2 = "{x+(y-[a+b]*c-[(d+e)]}";

    if (1 == areParenthesesBalanced(expr1))
    {
        printf("Expression 1 is Balanced.\n");
    }
    else
    {
        printf("Expression 1 is Not Balanced.\n");
    }

    if (1 == areParenthesesBalanced(expr2))
    {
        printf("Expression 2 is Balanced.\n");
    }
    else
    {
        printf("Expression 2 is Not Balanced.\n");
    }

    return 0;
}

/* --- Function Definitions --- */

int isMatchingPair(char _openChar, char _closeChar)
{
    if ('(' == _openChar && ')' == _closeChar)
    {
        return 1;
    }
    if ('{' == _openChar && '}' == _closeChar)
    {
        return 1;
    }
    if ('[' == _openChar && ']' == _closeChar)
    {
        return 1;
    }
    
    return 0;
}

int areParenthesesBalanced(const char* _expression)
{
    int i = 0;
    int isValid = 1;
    char poppedChar = '\0';
    Stack* stack = NULL;
    
    if (NULL == _expression)
    {
        return 0;
    }

    stack = StackCreate(10);
    
    if (NULL == stack)
    {
        return 0;
    }

    /* Loop until end of string or validation fails early */
    while ('\0' != _expression[i] && 1 == isValid)
    {
        if ('{' == _expression[i] || '(' == _expression[i] || '[' == _expression[i])
        {
            StackPush(stack, _expression[i]);
        }
        else if ('}' == _expression[i] || ')' == _expression[i] || ']' == _expression[i])
        {
            if (1 == StackIsEmpty(stack) || 
                STACK_SUCCESS != StackPop(stack, &poppedChar) || 
                0 == isMatchingPair(poppedChar, _expression[i]))
            {
                isValid = 0;
            }
        }
        i++;
    }

    /* Ensure no unclosed parentheses remain */
    if (1 == isValid && 0 == StackIsEmpty(stack))
    {
        isValid = 0;
    }

    StackDestroy(&stack);
    
    return isValid;
}