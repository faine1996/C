#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include "../inc/pingpoing.h"

int run_pingpong(void)
{
    int pipefd[2];
    int msgid;
    pid_t pid;

    if (0 != create_pipe(pipefd))
    {
        return EXIT_FAILURE;
    }

    msgid = create_msg_queue();
    if (-1 == msgid)
    {
        return EXIT_FAILURE;
    }

    pid = fork();
    if (-1 == pid)
    {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (0 == pid)
    {
        close(pipefd[1]);
        child_process(pipefd[0], msgid);
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    }
    else
    {
        close(pipefd[0]);
        parent_process(pipefd[1], msgid);
        close(pipefd[1]);
        
        wait(NULL);
        destroy_msg_queue(msgid);
    }

    return EXIT_SUCCESS;
}

int create_pipe(int pipefd[2])
{
    if (-1 == pipe(pipefd))
    {
        perror("pipe");
        return -1;
    }
    return 0;
}

int create_msg_queue(void)
{
    int msgid;
    
    msgid = msgget(MSG_KEY, IPC_CREAT | 0666);
    if (-1 == msgid)
    {
        perror("msgget");
    }
    
    return msgid;
}

void destroy_msg_queue(int msgid)
{
    if (-1 == msgctl(msgid, IPC_RMID, NULL))
    {
        perror("msgctl");
    }
}

void child_process(int pipe_read_fd, int msgid)
{
    char buffer[BUFFER_SIZE];
    int bytes_read;
    struct pingpong_msg msg;

    memset(buffer, 0, BUFFER_SIZE);
    bytes_read = read(pipe_read_fd, buffer, BUFFER_SIZE);

    if (0 < bytes_read)
    {
        printf("Child received via Pipe: %s\n", buffer);
    }

    msg.mtype = MSG_TYPE;
    strncpy(msg.mtext, "Pong", BUFFER_SIZE - 1);
    msg.mtext[BUFFER_SIZE - 1] = '\0';

    if (-1 == msgsnd(msgid, &msg, sizeof(msg.mtext), 0))
    {
        perror("msgsnd");
    }
}

void parent_process(int pipe_write_fd, int msgid)
{
    const char *ping_str = "Ping";
    struct pingpong_msg msg;
    int bytes_written;

    bytes_written = write(pipe_write_fd, ping_str, strlen(ping_str) + 1);
    if (-1 == bytes_written)
    {
        perror("write");
    }

    if (-1 != msgrcv(msgid, &msg, sizeof(msg.mtext), MSG_TYPE, 0))
    {
        printf("Parent received via Message Queue: %s\n", msg.mtext);
    }
    else
    {
        perror("msgrcv");
    }
}