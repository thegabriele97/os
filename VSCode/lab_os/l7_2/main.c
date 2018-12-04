#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define CODE_READY 0x1

pid_t pid_child;
bool child_exit = false;

void _hdl_p_int(int sig) {
    kill(pid_child, SIGINT);
}

void _hdl_c_int(int sig) {
    child_exit = true;
}

void child() {

    while(!child_exit) {
        pause();
    }

    printf("CHILD PID %d\n", getpid());
}

int main() {
    int _pipe_p[2], _pipe_c[2];
    char code = CODE_READY;

    pipe(_pipe_p);
    pipe(_pipe_c);

    if (!(pid_child = fork())) {

        signal(SIGINT, _hdl_c_int);
        write(_pipe_c[1], &code, 0x1);

        while(read(_pipe_p[0], &code, 0x1) > 0) {
            break;
        }

        close(_pipe_p[0]);
        close(_pipe_p[1]);
        close(_pipe_c[0]);
        close(_pipe_c[1]);

        child();
        exit(0);
    }

    signal(SIGINT, _hdl_p_int);
    write(_pipe_p[1], &code, 0x1);

    while(read(_pipe_c[0], &code, 0x1) > 0) {
        break;
    }

    close(_pipe_p[0]);
    close(_pipe_p[1]);
    close(_pipe_c[0]);
    close(_pipe_c[1]);

    wait((int *)0);

    return 0;

}