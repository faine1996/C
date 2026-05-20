#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080

int main()
{
    int sock, optval, back_log, client_sock, data_len, sent_bytes, expected_data_len, read_bytes;
    struct sockaddr_in sin, client_sin;
    socklen_t addr_len;
    char * data_addr = "Hello Client";
    char buffer[4096];
    /* Creating socket file distributer*/
    sock = socket(AF_INET, SOCK_STREAM,0);
    if (0 > sock)
    {
        perror("socket() failed to init");
        abort();
    }

    optval = 1;
    if (0> setsockopt(sock,SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
    {
        perror("Reuse port in server failed.\n");
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

    addr_len = sizeof(client_sin);
    client_sock = accept(sock, (struct sockaddr *) &client_sin, &addr_len);

    if (0 > client_sock)
    {
        perror("Accept failed");
        abort();
    }


    /* Receiving data */
    expected_data_len = sizeof(buffer) - 1;
    read_bytes = recv(client_sock, buffer, expected_data_len, 0);
    if (0 == read_bytes) 
    {
        printf("Client disconnected gracefully.\n");
    } 
    else if(0 > read_bytes) 
    {
        perror("recv failed");
    } 
    else 
    {
        buffer[read_bytes] = '\0';
        printf("Server received: %s\n", buffer);
    }

    /* Send data*/
    data_len = strlen(data_addr);
    sent_bytes = send(client_sock,data_addr, data_len, 0);

    if (0 > sent_bytes)
    {
        perror("Send failed");
    }


    close(client_sock);
    close(sock);

    return 0;
}