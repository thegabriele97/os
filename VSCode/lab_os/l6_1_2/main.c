#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#define CODE_KILL 0x00
#define CODE_GO 0x1

bool __exit = false;

void HDL_SIGUSR1(int sig) {
    __exit = true;
}

int main() {
    pid_t child_pid;
    int _pipe_p[2], _pipe_c[2], nR;
    char code;

    pipe(_pipe_c);
    pipe(_pipe_p);

    if (child_pid = fork()) {

        signal(SIGUSR1, HDL_SIGUSR1);

        close(_pipe_p[0]);
        close(_pipe_c[1]);

        do {
            if (__exit) {
                code = CODE_KILL;
                printf("Sending CODE 0x%x\n", code);
                fflush(stdout);
                write(_pipe_p[1], &code, 0x1);

                waitpid(child_pid, NULL, 0);
                break;
            }

            printf("I am the father (PID=%d)\n", getpid());
            fflush(stdout);

            code = CODE_GO;
            write(_pipe_p[1], &code, 0x1);
        } while ((nR = read(_pipe_c[0], &code, 0x1)));

    } else {

        signal(SIGUSR1, SIG_IGN);

        close(_pipe_c[0]);
        close(_pipe_p[1]);

        while ((nR = read(_pipe_p[0], &code, 0x1)) > 0) {

            if (code == CODE_KILL) {
                printf("Received CODE 0x%x\n", code);
                fflush(stdout);
                
                raise(SIGILL);
            }

            printf("I am the child (PID=%d)\n", getpid());
            fflush(stdout);

            code = CODE_GO;
            write(_pipe_c[1], &code, 0x1);
        }

        exit(0);
    }

    return 0;
}