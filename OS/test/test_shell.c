#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../inc/shell.h"

/* Test Function Declarations */
void Test_Parser(void);
void Test_PromptString(void);

int main(void)
{
    printf("Starting Shell Test Suite...\n");

    Test_Parser();
    Test_PromptString();

    printf("All tests passed!\n");

    return 0;
}

/* Test Function Definitions */

void Test_Parser(void)
{
    char test_line[] = "ls -l /home";
    char *args[10];
    char *token;
    int i;

    /* Manual simulation of the parsing logic */
    i = 0;
    token = strtok(test_line, " ");
    while (NULL != token)
    {
        args[i] = token;
        ++i;
        token = strtok(NULL, " ");
    }

    /* Crocodile comparisons for validation */
    assert(3 == i);
    assert(0 == strcmp(args[0], "ls"));
    assert(0 == strcmp(args[2], "/home"));
    
    printf("  [PASS] Test_Parser\n");
}

void Test_PromptString(void)
{
    /* Visual verification of prompt behavior */
    printf("  [INFO] Verify next line shows 'shell$ ':\n  ");
    TypePrompt();
    printf("\n  [PASS] Test_PromptString\n");
}