#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

bool startProg = false;

void HDL_SSIGUSR1(int sig) {
    printf("Child Wake-up\n");
    fflush(stdout);
}

void HDL_PSIGUSR1(int sig) {
    printf("Father Wake-up\n");
    fflush(stdout);
}

void HDL_PSIGALARM(int sig) {
    startProg = true;
}

int main() {
    pid_t pid;

    signal(SIGALRM, HDL_PSIGALARM);

    if (pid = fork()) {
        signal(SIGUSR1, HDL_PSIGUSR1);

        while(!startProg) {
            pause();
        }

        signal(SIGALRM, SIG_DFL);
    } else {
        signal(SIGUSR1, HDL_SSIGUSR1);
        signal(SIGALRM, SIG_DFL);
        kill(getppid(), SIGALRM);

        while(true) {
            pause();
            kill(getppid(), SIGUSR1);
        }

        exit(0);
    }

    while(true) {
        kill(pid, SIGUSR1);
        pause();
        sleep(1);
    }

    return 0;
}