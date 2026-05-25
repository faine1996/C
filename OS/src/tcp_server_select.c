#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h> /* Required for select() and fd_set */
#include <errno.h>

#define PORT 8080
#define MAX_CLIENTS 10

int main(void)
{
    /* --- Master Socket & Infrastructure Variables --- */
    int sock, optval, back_log, i;
    int client_sockets[MAX_CLIENTS];
    struct sockaddr_in sin, client_sin;
    socklen_t addr_len;
    
    /* --- Multiplexing Variables --- */
    fd_set read_fds;
    int max_fd, activity, sd;
    
    /* --- I/O Variables --- */
    int new_sock, read_bytes, sent_bytes;
    char buffer[4096];
    char *reply_msg = "Hello from Select Server\n";

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
    if (0 > setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
    {
        perror("SetSock() failed.\n");
        abort();
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(PORT);
    
    if (0 > bind(sock, (struct sockaddr *)&sin, sizeof(sin)))
    {
        perror("Bind Failed!");
        abort();
    }

    back_log = 5;
    if (0 > listen(sock, back_log))
    {
        perror("Listen Failed");
        abort();
    }

    printf("Select Server is awake and listening on port %d...\n", PORT);

    while(1)
    {
        /* 1. Reset the fd_set bitmask */
        FD_ZERO(&read_fds);

        /* 2. Add the master socket to the set */
        FD_SET(sock, &read_fds);
        max_fd = sock;

        /* 3. Add all valid child sockets to the set */
        for (i = 0; i < MAX_CLIENTS; ++i)
        {
            sd = client_sockets[i];
            
            if (sd != -1)
            {
                FD_SET(sd, &read_fds);
            }
            
            /* max_fd must track the highest file descriptor number for select() */
            if (sd > max_fd)
            {
                max_fd = sd;
            }
        }

        /* 4. The Sleep Call - Blocks until activity occurs on ANY socket in the set */
        activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);

        if ((0 > activity) && (errno != EINTR))
        {
            perror("Select error");
        }

        /* 5. Check if the master socket woke us up (New Connection) */
        if (FD_ISSET(sock, &read_fds))
        {
            addr_len = sizeof(client_sin);
            new_sock = accept(sock, (struct sockaddr *)&client_sin, &addr_len);
            
            if (new_sock >= 0)
            {
                for (i = 0; i < MAX_CLIENTS; ++i)
                {
                    if (client_sockets[i] == -1)
                    {
                        client_sockets[i] = new_sock;
                        printf("Assigned new client to slot %d\n", i);
                        break;
                    }
                }
                
                if (i == MAX_CLIENTS)
                {
                    printf("Server full! Rejecting connection.\n");
                    close(new_sock);
                }
            }
        }

        /* 6. Check if a client socket woke us up (Incoming Data or Disconnect) */
        for (i = 0; i < MAX_CLIENTS; ++i)
        {
            sd = client_sockets[i];
            
            if (sd != -1 && FD_ISSET(sd, &read_fds))
            {
                read_bytes = recv(sd, buffer, sizeof(buffer) - 1, 0);
                
                if (0 == read_bytes)
                {
                    printf("Client in slot %d disconnected gracefully.\n", i);
                    close(sd);
                    client_sockets[i] = -1;
                }
                else if (0 > read_bytes)
                {
                    perror("Recv system error");
                    close(sd);
                    client_sockets[i] = -1;
                }
                else
                {
                    buffer[read_bytes] = '\0';
                    printf("Server received from slot %d: %s\n", i, buffer);
                    
                    sent_bytes = send(sd, reply_msg, strlen(reply_msg), 0);
                    if (0 > sent_bytes)
                    {
                        perror("Send reply failed");
                    }
                }
            }
        }
    }
    
    return 0;
}