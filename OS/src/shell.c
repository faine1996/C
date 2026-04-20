#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../inc/shell.h"

#define MAX_LINE 1024
#define MAX_ARGS 64

/* Internal Helper Declarations */
static void HandleCommand(char *line);
static int ParseLine(char *line, char **args);
static void Execute(char **args);

/* Public Function: Entry point for the shell logic */
void RunShell(void)
{
    char line[MAX_LINE];

    while (1)
    {
        TypePrompt();

        /* Get input via fgets as instructed */
        if (NULL == fgets(line, MAX_LINE, stdin))
        {
            break;
        }

        /* Remove trailing newline character */
        line[strcspn(line, "\n")] = '\0';

        /* Check for exit command using strcmp before fork [cite: 186-187] */
        if (0 == strcmp(line, "exit"))
        {
            break;
        }

        HandleCommand(line);
    }
}

void TypePrompt(void)
{
    printf("shell$ ");
    fflush(stdout);
}

/* Helper Definitions */

static void HandleCommand(char *line)
{
    char *args[MAX_ARGS];
    int arg_count;

    arg_count = ParseLine(line, args);

    if (0 < arg_count)
    {
        Execute(args);
    }
}

static int ParseLine(char *line, char **args)
{
    char *token;
    int i;

    i = 0;
    token = strtok(line, " ");

    while (NULL != token && i < MAX_ARGS - 1)
    {
        args[i] = token;
        ++i;
        token = strtok(NULL, " ");
    }
    args[i] = NULL; /* Required for execvp compatibility */

    return i;
}

static void Execute(char **args)
{
    pid_t pid;
    int status;

    pid = fork();

    if (0 > pid)
    {
        perror("Fork failed");
    }
    else if (0 == pid)
    {
        /* Child process: Inherits resources [cite: 124] but replaces memory image [cite: 126] */
        if (0 > execvp(args[0], args))
        {
            printf("Invalid command.\n");
            exit(1);
        }
    }
    else
    {
        /* Parent process: Waits for child to avoid zombies [cite: 150-154] */
        waitpid(pid, &status, 0);
    }
}