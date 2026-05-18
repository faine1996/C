#ifndef PINGPONG_H
#define PINGPONG_H

#define MSG_KEY 0x1234
#define MSG_TYPE 1
#define BUFFER_SIZE 16

struct pingpong_msg
{
    long mtype;
    char mtext[BUFFER_SIZE];
};

int create_pipe(int pipefd[2]);
int create_msg_queue(void);
void destroy_msg_queue(int msgid);
void child_process(int pipe_read_fd, int msgid);
void parent_process(int pipe_write_fd, int msgid);
int run_pingpong(void);

#endif