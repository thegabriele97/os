#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

bool startProg = false;

void HDL_PSIGUSR(int sig) {

    if (!startProg) {
        startProg = true;
        return;
    }

    printf("Received signal 0x%x\n", sig);
    fflush(stdout);
}

void HDL_SSIGUSR(int sig) {
    kill(getppid(), (sig == SIGUSR1) ? SIGUSR2 : SIGUSR1);
}

void HDL_SSIGINT(int sig) {
    kill(getppid(), SIGINT);
}

void son() {

    signal(SIGUSR1, HDL_SSIGUSR);
    signal(SIGUSR2, HDL_SSIGUSR);
    signal(SIGINT, HDL_SSIGINT);
    signal(SIGALRM, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);

    kill(getppid(), SIGUSR1);

    while (true) {
        pause();
    }

}

int main() {
    int* _buf, n, i, t;
    pid_t pid;

    printf("Number of signals to send: ");
    scanf("%d", &n);

    printf("Interval time: ");
    scanf("%d", &t);

    printf("Signals to send: ");

    _buf = malloc(n * sizeof *_buf);
    for (i = 0; i < n; i++) {
        scanf("%d", &_buf[i]);
    }

    fflush(stdout);
 
    if (pid = fork()) {

        signal(SIGUSR1, HDL_PSIGUSR);
        signal(SIGUSR2, HDL_PSIGUSR);

        while (!startProg) {
            pause();
        }

        i = 0;
        while (true) {
            i %= n;

            kill(pid, _buf[i++]);
            pause();
            sleep(t);
        }

    } else {
        son();
        exit(0);
    }

    waitpid(pid, (int*)0, 0);
    return 0;
}