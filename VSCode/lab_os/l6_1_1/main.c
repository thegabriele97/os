#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>

#define _PIPE_OUT 0x0
#define _PIPE_IN 0x1
#define BUFFSIZ 0x200

void producer(int fdOut) {
    char buf[BUFFSIZ] = { 0 };
    int nR;

    while ((nR = read(STDIN_FILENO, buf, BUFFSIZ)) > 0x0) {
        write(fdOut, buf, nR);

        if (buf[nR - 4] == 'e' && buf[nR - 3] == 'n' && buf[nR - 2] == 'd') {
            raise(SIGILL);
        }
    }
}

void consumer(int fdIn) {
    char buf[BUFFSIZ];
    int nR, i;

    while ((nR = read(fdIn, buf, BUFFSIZ)) > 0x0) {

        for (i = 0; i < nR; i++) {
            buf[i] = toupper(buf[i]);
        }

        write(STDOUT_FILENO, buf, nR);

        if (buf[nR - 4] == 'E' && buf[nR - 3] == 'N' && buf[nR - 2] == 'D') {
            raise(SIGILL);
        }
    }
}

int main() {
    pid_t pid_producer, pid_consumer;
    int _pipe_fd[2];

    pipe(_pipe_fd);

    if (pid_producer = fork()) {
        if (!(pid_consumer = fork())) {
            close(_pipe_fd[_PIPE_IN]);
            
            
            consumer(_pipe_fd[_PIPE_OUT]);
            exit(0);
        }
    } else {
        close(_pipe_fd[_PIPE_OUT]);

        producer(_pipe_fd[_PIPE_IN]);
        exit(0);
    }

    waitpid(pid_producer,(int *)0, 0x0);
    waitpid(pid_consumer,(int *)0, 0x0);

    return 0;
}