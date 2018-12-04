#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#define BUFFSIZ 0x200
#define CODE_END 0x1

void child(int fd_out) {
    char buf[BUFFSIZ];
    int fd_in, nR;
    
    fd_in = open("data", O_RDONLY);

    while ((nR = read(fd_in, buf, BUFFSIZ - 1)) > 0) {
        write(fd_out, buf, nR);
    }

    close(fd_in);
}

int main() {
    int _pipe[2];
    char code;

    pipe(_pipe);

    if (!fork()) {
        close(_pipe[0]);
        child(_pipe[1]);
        exit(0);
    }

    close(_pipe[1]);

    while(read(_pipe[0], &code, 0x1) > 0) {

        if (code == CODE_END) {
            break;
        }

        write(STDOUT_FILENO, &code, 0x1);
        sleep(1);
    }

    printf("\n");

    close(_pipe[0]);
    return 0;
}