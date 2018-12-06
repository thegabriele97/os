#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFSIZ 0x400
#define CODE_GO_2ndCOMM 0x1
#define CODE_GO_FORWARD 0x2

int main() {
    int _pipe_fd[2], _pipe_ack[2], _pipe_ack2[2], i;
    char buf[BUFFSIZ], code;
    pid_t pid;

    pipe(_pipe_fd);
    pipe(_pipe_ack);
    pipe(_pipe_ack2);

    if (!fork()) {
        //CHILD 1
        close(_pipe_fd[0]); //pipe is write-only for this process

        do {
            printf("Enter command 1 (incl. args) or quit: ");
            fflush(stdout);
            fgets(buf, BUFFSIZ, stdin);

            code = CODE_GO_2ndCOMM;
            write(_pipe_ack[1], &code, 0x1);

            if (!fork()) {
                //exec child1
                dup2(_pipe_fd[1], STDOUT_FILENO);
                system(buf);
            }

            while (read(_pipe_ack2[0], &code, 0x1) > 0) {
                break;
            }

        } while (true);

        exit(0);
    } else {
        if (fork()) {
            //CHILD 2
            close(_pipe_fd[1]); //pipe is read-only for this process

            do {
                while(read(_pipe_ack[0], &code, 0x1) > 0) {
                    break;
                }

                printf("Enter command 2 (incl. args): ");
                fflush(stdout);
                fgets(buf, BUFFSIZ, stdin);

                if (!(pid = fork())) {
                    //exec child
                    dup2(_pipe_fd[0], STDIN_FILENO);
                    system(buf);
                }

                waitpid(pid, (int *)0, 0);

                code = CODE_GO_FORWARD;
                write(_pipe_ack2[1], &code, 0x1);

            } while (true);

            exit(0);
        }
    }

    //PARENT
    while(wait((int *)0) > 0);

    return 0;
}