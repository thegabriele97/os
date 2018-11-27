#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

bool startProg;
bool closeProg;

void HDL_SIG(int sig) {
    printf("Parent PID: %d. Child PID: %d. SIG: 0x%x\n", getppid(), getpid(), sig);

    closeProg = true;
    fflush(stdout);
}

void HDL_PSIGUSR2(int sig) {
    startProg = true;
}

int main(int argc, char** argv) {
    int n, i;
    pid_t pid, *_pid_buf;

    if (argc <= 1) {
        return 1;
    }

    n = atoi(argv[1]);
    _pid_buf = malloc(n * sizeof *_pid_buf);

    for (i = 0; i < n; i++) {
        startProg = false;

        if (!(pid = fork())) {
            signal(SIGUSR1, HDL_SIG);
            kill(getppid(), SIGUSR2);

            while(true) {
                pause;
            }

            exit(0);
        } else {
            signal(SIGUSR2, HDL_PSIGUSR2);
            _pid_buf[i] = pid;

            while(!startProg) {
                pause();
            }
        }
    }

    while(true) {
        for (i = 0; i < n; i++) {
            printf("Parent PID: %d. Child PID: %d\n", getpid(), _pid_buf[i]);
            fflush(stdout);
            
            kill(_pid_buf[i], SIGUSR1);
            sleep(2);
        }
    }
     
    free(_pid_buf);
    return 0;
}