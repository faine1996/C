#include <stdio.h>
#include <stdlib.h>
#include "../inc/pingpoing.h"

int main(void)
{
    if (EXIT_SUCCESS != run_pingpong())
    {
        fprintf(stderr, "Ping pong test failed.\n");
        return EXIT_FAILURE;
    }

    printf("Ping pong test completed successfully.\n");
    return EXIT_SUCCESS;
}