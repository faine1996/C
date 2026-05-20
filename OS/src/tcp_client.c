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
    int sock, optval, data_len, sent_bytes, expected_data_len, read_bytes;
    struct sockaddr_in sin;
    char * data_addr = "Hello Server";
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
        perror("Reuse port in client failed.\n");
        abort();
    }

    /* Bind client port number*/
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    sin.sin_port = htons(PORT);

    if (0 > connect(sock, (struct sockaddr *) &sin, sizeof(sin)))
    {
        perror("Connection failed");
        abort();
    }

    /* Send data*/
    data_len = strlen(data_addr);
    sent_bytes = send(sock,data_addr, data_len, 0);

    if (0 > sent_bytes)
    {
        perror("Send failed");
    }

    expected_data_len = sizeof(buffer);

    /* Receiving data */
    read_bytes = recv(sock,buffer,expected_data_len, 0);
    if (0 == read_bytes)
    {
        printf("Server disconnected.\n");
    }
    else if(0> read_bytes)
    {
        perror("recv failed");
    }
    else
    {
        buffer[read_bytes] = '\0';
        printf("Received: %s\n", buffer);
    }

    close(sock);



    return 0;
}