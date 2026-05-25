#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080

/* --- Forward Declarations (Prototypes) --- */
void run_stochastic_test(void);
void delay_execution(void);


/* --- Main --- */
int main(void)
{
    printf("Starting Stochastic Client Simulator...\n");
    
    run_stochastic_test();
    
    return 0;
}


/* --- Test Definitions --- */

void delay_execution(void)
{
    /* Volatile prevents the compiler from optimizing out this empty loop */
    volatile int delay;
    
    /* ANSI C compatible throttle loop (approx 0.1s - 0.3s delay depending on CPU) */
    for(delay = 0; delay < 20000000; ++delay) 
    {
    }
}

void run_stochastic_test(void)
{
    int sock;
    int is_connected;
    int action;
    int sent_bytes;
    int read_bytes;
    struct sockaddr_in sin;
    char * data_addr = "Stochastic Ping";
    char buffer[4096];

    /* Strict ANSI C Initialization */
    is_connected = 0;
    sock = -1;
    srand(time(NULL));

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    sin.sin_port = htons(PORT);

    while (1)
    {
        /* Generate a random percentage between 0 and 99 */
        action = rand() % 100;

        if (0 == is_connected)
        {
            /* 30% chance to connect */
            if (action < 30)
            {
                sock = socket(AF_INET, SOCK_STREAM, 0);
                if (sock >= 0)
                {
                    if (0 == connect(sock, (struct sockaddr *)&sin, sizeof(sin)))
                    {
                        printf("[State] CONNECTED to server.\n");
                        is_connected = 1;
                    }
                    else
                    {
                        close(sock);
                    }
                }
            }
        }
        else
        {
            /* 5% chance to disconnect */
            if (action < 5)
            {
                printf("[State] DISCONNECTING intentionally.\n");
                close(sock);
                is_connected = 0;
                sock = -1;
            }
            /* 30% chance to send/receive */
            else if (action >= 5 && action < 35) 
            {
                sent_bytes = send(sock, data_addr, strlen(data_addr), 0);
                if (sent_bytes > 0)
                {
                    read_bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
                    if (0 == read_bytes)
                    {
                        printf("[Event] Server dropped the connection.\n");
                        close(sock);
                        is_connected = 0;
                        sock = -1;
                    }
                    else if (read_bytes > 0)
                    {
                        printf("[Event] Exchanged Ping-Pong data successfully.\n");
                    }
                }
                else
                {
                    /* If send fails, the pipeline is broken */
                    close(sock);
                    is_connected = 0;
                    sock = -1;
                }
            }
            /* Remaining 65% chance: do nothing and stay idle */
        }

        /* Throttle execution so you can read the terminal output */
        delay_execution();
    }
}