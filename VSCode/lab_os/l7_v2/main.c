#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFSIZ 0x1000
#define CODE_GONE 0x1

void child(const char* file_name, int fdOut) {
    FILE* fp;
    char buf[BUFFSIZ];
    int i;
    
    fp = fopen(file_name, "r");

    while (fgets(buf, BUFFSIZ - 1, fp) != NULL) {
        write(fdOut, buf, (strlen(buf) + 1) * sizeof *buf);
    }

    fclose(fp);
}

int main() {
    int _pipe_fd[2], i, child_closed = 0;   
    char *file_name[2] = {"file_1.txt", "file_2.txt"}, code = CODE_GONE;

    pipe(_pipe_fd);

    for (i = 0; i < 2; i++) {

        if (!fork()) {
            close(_pipe_fd[0]);
            child(file_name[i], _pipe_fd[1]);
            write(_pipe_fd[1], &code, 0x1);
            exit(0);
        }
    }

    while (child_closed < 2 && read(_pipe_fd[0], &code, 0x1) > 0) {
        
        if (code == CODE_GONE) {
            child_closed++;
            continue;
        }

        write(STDOUT_FILENO, &code, 0x1);
    }

    while (wait((int *)0) > 0);
    printf("\n");

    close(_pipe_fd[0]);
    close(_pipe_fd[1]);
    return 0;
}