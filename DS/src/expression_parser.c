#include <stdio.h>
#include <stdlib.h>
#include "../inc/expression_parser.h"
#include "../inc/adt_stack.h" 

/* --- Helper Declarations --- */
static int IsOperand(int _ch);
static int GetPrecedence(int _operator);
static int Calculate(int _left, int _right, int _operator);
static int PowerInt(int _base, int _exp);

/* --- Main Functions --- */

int InfixToPostfix(const char* _infix, char* _postfix)
{
    Stack* opStack = NULL;
    int i = 0;
    int j = 0;
    int token;
    int poppedOp;

    if (NULL == _infix || NULL == _postfix)
    {
        return 0; 
    }

    opStack = StackCreate(20);
    if (NULL == opStack)
    {
        return 0;
    }

    while ('\0' != _infix[i])
    {
        token = _infix[i]; 

        if (1 == IsOperand(token))
        {
            _postfix[j++] = (char)token;
        }
        else if ('(' == token)
        {
            StackPush(opStack, token);
        }
        else if (')' == token)
        {
            while (0 == StackIsEmpty(opStack))
            {
                StackPop(opStack, &poppedOp);
                
                if ('(' == poppedOp)
                {
                    break;
                }
                _postfix[j++] = (char)poppedOp;
            }
        }
        else 
        {
            while (0 == StackIsEmpty(opStack))
            {
                StackPop(opStack, &poppedOp);

                /* Right-associativity logic applied here */
                if ('(' == poppedOp || 
                    GetPrecedence(token) > GetPrecedence(poppedOp) ||
                    (GetPrecedence(token) == GetPrecedence(poppedOp) && '^' == token))
                {
                    StackPush(opStack, poppedOp);
                    break;
                }
                
                _postfix[j++] = (char)poppedOp;
            }
            StackPush(opStack, token);
        }
        
        i++; 
    }

    while (0 == StackIsEmpty(opStack))
    {
        StackPop(opStack, &poppedOp);
        _postfix[j++] = (char)poppedOp;
    }

    _postfix[j] = '\0';
    StackDestroy(&opStack);

    return 1;
}

int EvaluatePostfix(const char* _postfix, int* _result)
{
    Stack* evalStack = NULL;
    int i = 0;
    int token;
    int itemLeft;
    int itemRight;
    int tempRes;

    if (NULL == _postfix || NULL == _result)
    {
        return 0;
    }

    evalStack = StackCreate(20);
    
    while ('\0' != _postfix[i])
    {
        token = _postfix[i];

        if (1 == IsOperand(token))
        {
            /* Convert ASCII char digit to actual integer value */
            StackPush(evalStack, token - '0'); 
        }
        else
        {
            StackPop(evalStack, &itemRight);
            StackPop(evalStack, &itemLeft);

            /* Protection against division by zero */
            if ('/' == token && 0 == itemRight)
            {
                StackDestroy(&evalStack);
                return 0; 
            }

            tempRes = Calculate(itemLeft, itemRight, token);
            StackPush(evalStack, tempRes);
        }
        i++;
    }

    StackPop(evalStack, &itemLeft);
    *_result = itemLeft;

    StackDestroy(&evalStack);
    return 1;
}

/* --- Helper Implementations --- */

static int IsOperand(int _ch)
{
    if (_ch >= '0' && _ch <= '9')
    {
        return 1;
    }
    return 0;
}

static int GetPrecedence(int _operator)
{
    if ('^' == _operator)
    {
        return 3;
    }
    if ('*' == _operator || '/' == _operator)
    {
        return 2;
    }
    if ('+' == _operator || '-' == _operator)
    {
        return 1;
    }
    return 0;
}

static int Calculate(int _left, int _right, int _operator)
{
    if ('+' == _operator) return _left + _right;
    if ('-' == _operator) return _left - _right;
    if ('*' == _operator) return _left * _right;
    if ('/' == _operator) return _left / _right;
    if ('^' == _operator) return PowerInt(_left, _right);
    return 0;
}

static int PowerInt(int _base, int _exp)
{
    int result = 1;
    int k;
    for (k = 0; k < _exp; k++)
    {
        result *= _base;
    }
    return result;
}