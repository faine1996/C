#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/text_ex.h"

#define MAX_LINE_LEN 1024

void tail(FILE* in, int n)
{
    char **ring_buffer;
    char temp_line[MAX_LINE_LEN];
    int current = 0;
    int total_lines = 0;
    int i, start, count;

    /* Handle edge case where n is 0 or negative */
    if (0 >= n)
    {
        return;
    }

    /* 1. Allocate the circular buffer (Array of strings) */
    ring_buffer = (char **)malloc(n * sizeof(char *));
    if (ring_buffer == NULL)
    {
        perror("Failed to allocate string array");
        return;
    }

    for (i = 0; i < n; ++i)
    {
        ring_buffer[i] = (char *)malloc(MAX_LINE_LEN * sizeof(char));
        if (ring_buffer[i] == NULL)
        {
            perror("Failed to allocate line buffer");
            /* Free previously allocated memory before exiting */
            while (--i >= 0)
            {
                free(ring_buffer[i]);
            }
            free(ring_buffer);
            return;
        }
        ring_buffer[i][0] = '\0'; /* Initialize empty */
    }

    /* 2. Read the file forward line by line */
    while (NULL != fgets(temp_line, sizeof(temp_line), in))
    {
        /* Copy the line into the current slot of our ring buffer */
        strncpy(ring_buffer[current], temp_line, MAX_LINE_LEN - 1);
        ring_buffer[current][MAX_LINE_LEN - 1] = '\0'; /* Ensure null termination */

        /* Advance the index, wrapping around to 0 if we hit n */
        current = (current + 1) % n;
        total_lines++;
    }

    /* 3. Determine where to start printing */
    /* If we read fewer lines than n, start at 0. Otherwise, start at 'current' */
    if (total_lines < n)
    {
        start = 0;
        count = total_lines;
    }
    else
    {
        start = current;
        count = n;
    }

    /* 4. Print the last 'n' lines */
    printf("Printing last %d lines -\n", count);
    for (i = 0; i < count; ++i)
    {
        printf("%s", ring_buffer[(start + i) % n]);
    }
    printf("\n\n");

    /* 5. Clean up allocated memory */
    for (i = 0; i < n; ++i)
    {
        free(ring_buffer[i]);
    }
    free(ring_buffer);
}