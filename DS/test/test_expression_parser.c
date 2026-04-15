#include <stdio.h>
#include <string.h>
#include "expression_parser.h"

/* --- Test Helper Declarations --- */
static void RunTest(const char* _testName, const char* _infix, const char* _expectedPost, int _expectedResult);
static void TestErrors(void);

/* --- Main Function --- */
int main(void)
{
    printf("--- Running Robust Expression Parser Tests ---\n\n");
    
    /* 1. Basic Precedence (From Slide 20) */
    RunTest("Basic Precedence", "3+2*4", "324*+", 11);
    
    /* 2. Parentheses & Over 127 limit (From Slide 23 variant) */
    RunTest("Parentheses Math", "5*(6+2)-8/4", "562+*84/-", 38);
    
    /* 3. Right-Associativity Exponents: 2^(3^2) = 512, NOT (2^3)^2 = 64 */
    RunTest("Right-Associative Exponents", "2^3^2", "232^^", 512);

    /* 4. Error Handling Checks */
    TestErrors();

    printf("\n--- Tests Complete ---\n");
    return 0;
}

/* --- Test Helper Implementations --- */

static void RunTest(const char* _testName, const char* _infix, const char* _expectedPost, int _expectedResult)
{
    char postfixBuffer[50];
    int result = 0;

    printf("[Test] %s: '%s'\n", _testName, _infix);
    
    if (1 == InfixToPostfix(_infix, postfixBuffer))
    {
        if (0 == strcmp(postfixBuffer, _expectedPost))
        {
            printf("  [PASS] Parse: %s -> %s\n", _infix, postfixBuffer);
        }
        else
        {
            printf("  [FAIL] Parse: Expected %s, got %s\n", _expectedPost, postfixBuffer);
        }
    }
    else
    {
        printf("  [FAIL] Parse returned error.\n");
        return;
    }

    if (1 == EvaluatePostfix(postfixBuffer, &result))
    {
        if (_expectedResult == result)
        {
            printf("  [PASS] Eval:  %s = %d\n", postfixBuffer, result);
        }
        else
        {
            printf("  [FAIL] Eval:  Expected %d, got %d\n", _expectedResult, result);
        }
    }
    else
    {
        printf("  [FAIL] Eval returned error.\n");
    }
    printf("\n");
}

static void TestErrors(void)
{
    int result;
    char buffer[50];
    
    printf("[Test] Error Handling\n");

    /* Division by zero test */
    if (0 == EvaluatePostfix("50/", &result))
    {
        printf("  [PASS] Gracefully caught division by zero.\n");
    }
    else
    {
        printf("  [FAIL] Did not catch division by zero.\n");
    }

    /* NULL Pointer checks */
    if (0 == InfixToPostfix(NULL, buffer) && 0 == EvaluatePostfix(NULL, &result))
    {
        printf("  [PASS] Gracefully caught NULL pointers.\n");
    }
    else
    {
        printf("  [FAIL] Did not catch NULL pointers.\n");
    }
}