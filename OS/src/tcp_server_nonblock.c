#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>   /* Required for fcntl() */
#include <errno.h>   /* Required to check EAGAIN / EWOULDBLOCK */

#define PORT 8080
#define MAX_CLIENTS 10

int main()
{
    int sock, optval, back_log, client_sockets[MAX_CLIENTS], i, flags, new_sock, read_bytes, sent_bytes;
    struct sockaddr_in sin, client_sin;
    socklen_t addr_len;
    char *reply_msg = "Hello from Non-Blocking server\n";
    char buffer[4096];
    int last_checked_index;
    int checked_count;
    int current_index;

    /* Initialize client sockets tracking array to empty (-1) */

    for (i = 0; i < MAX_CLIENTS; ++i)
    {
        client_sockets[i] = -1;
    }

    /* Standard Master Socket Creation */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (0 > sock)
    {
        perror("Master Socket() failed");
        abort();
    }

    optval = 1;
    if (0 > setsockopt(sock,SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
    {
        perror("SetSock() in server failed.\n");
        abort();
    }

    /* Bind server port number*/
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(PORT);
    
    if(0 > bind(sock,(struct sockaddr *)&sin, sizeof(sin)))
    {
        perror("Bind Failed!");
        abort();
    }

    back_log = 5;
    if (0 > listen(sock,back_log))
    {
        perror("Listen Failed;");
        abort();
    }

    /* Get current socket flags */
    flags = fcntl(sock,F_GETFL,0);
    if (0 > flags)
    {
        perror("fcnlt F_GETFL failed");
        abort();
    }

    /* Set socket flags to include O_NONBLOCK */
    if (0 > fcntl(sock, F_SETFL, flags | O_NONBLOCK))
    {
        perror("fcnet F_SETFL failed");
        abort();
    }
    last_checked_index = 0;
    while(1)
    {
        /*Try to accept a new connection */
        addr_len = sizeof(client_sin);
        new_sock = accept(sock, (struct sockaddr *) &client_sin, &addr_len);

        if (0 <= new_sock)
        {
            /* Make the new client socket non-blocking before saving it */
            flags = fcntl(new_sock,F_GETFL,0);
            if (0 <= flags)
            {
                fcntl(new_sock, F_SETFL, flags | O_NONBLOCK);
            }
            /* Find the first empty slot (-1) and save the socket */
            for (i = 0; i < MAX_CLIENTS; ++i)
            {
                if (-1 == client_sockets[i])
                {
                    client_sockets[i] = new_sock;
                    printf("Assigned client to slot %d\n", i);
                    break;
                }
            }
            /* If we looped through the entire array and didn't find a -1 */
            if (i == MAX_CLIENTS)
            {
                printf("Server full! Rejecting connection.\n");
                close(new_sock);
            }
        }
        else
        {
            /* Failures other than "no data waiting" should be evaluated */
            if (errno != EAGAIN && errno != EWOULDBLOCK)
            {
                perror("Accept system error");
            }
        }
        for (checked_count = 0; checked_count < MAX_CLIENTS; ++checked_count)
        {
            current_index = (last_checked_index + checked_count) % MAX_CLIENTS;
            
            if (-1 != client_sockets[current_index])
            {
                read_bytes = recv(client_sockets[current_index], buffer, sizeof(buffer) - 1, 0);

                if (0 == read_bytes) 
                {
                    printf("Client in slot %d disconnected gracefully.\n", current_index);
                    close(client_sockets[current_index]);
                    client_sockets[current_index] = -1;
                } 
                else if (0 > read_bytes) 
                {
                    /* Ignore the block notices; only report structural failures */
                    if (errno != EAGAIN && errno != EWOULDBLOCK)
                    {
                        perror("Recv system error");
                        close(client_sockets[current_index]);
                        client_sockets[current_index] = -1;
                    }
                } 
                else 
                {
                    buffer[read_bytes] = '\0';
                    printf("Server received from slot %d: %s\n", current_index, buffer);
                    
                    /* Send Ping-Pong reply back to client */
                    sent_bytes = send(client_sockets[current_index], reply_msg, strlen(reply_msg), 0);
                    if (0 > sent_bytes)
                    {
                        perror("Send reply failed");
                    }
                }
            }
        }
        
        /* Update our round-robin cursor variable for the next run */
        last_checked_index = (last_checked_index + 1) % MAX_CLIENTS;
    }
    return 0;
}