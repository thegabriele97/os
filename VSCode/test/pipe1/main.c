#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PIPE_OUT 0x0
#define PIPE_IN 0x1
#define BUFSIZ1 4
#define BUFSIZ2 1

void child2(int fdOut) {
    int nR;
    char buf[BUFSIZ2];
    
    printf("Child > stdout PID %d\n", getpid());

    while ((nR = read(fdOut, buf, BUFSIZ2)) > 0) {

        if (buf[0] == '\0') {
            break;
        }

        write(STDOUT_FILENO, buf, nR);
    }
}

void child1(int fdIn) {
    int fd, nR;
    char buf[BUFSIZ1];

    printf("Child < in File PID %d\n", getpid());

    fd = open("in", O_RDONLY);

    while ((nR = read(fd, buf, BUFSIZ1)) > 0) {
        write(fdIn, buf, nR);
    }

    write(fdIn, "\0", 2 * sizeof(char));

    close(fd);
}

int main (int argc, char **argv) {
    int _pipe_fd[2];

    pipe(_pipe_fd);

    if (fork()) {
        if (!fork()) {
            close(_pipe_fd[PIPE_OUT]);
            child1(_pipe_fd[PIPE_IN]);
            exit(0);
        }
    } else {
        close(_pipe_fd[PIPE_IN]);
        child2(_pipe_fd[PIPE_OUT]);
        exit(0);
    }

    while (wait((int *)0) > 0);
    return 0;
}