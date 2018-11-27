#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void HDL_PSIGUSR(int sig) {
    printf("Signal 0x%x received.\n\n", sig);
    fflush(stdout);
}

void HDL_SSIGUSR(int sig) {
    if (sig == SIGUSR1) {
        printf("PID %d -> PID %d: 0x%x\n", getpid(), getppid(), SIGUSR2);
        kill(getppid(), SIGUSR2);
    } else if (sig == SIGUSR2) {
        printf("PID %d -> PID %d: 0x%x\n", getpid(), getppid(), SIGUSR1);
        kill(getppid(), SIGUSR1);
    }

    fflush(stdout);
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

    while (true) {
        pause();
    }

}

int main() {
    int* _buf, n, i, t, timer = 0;
    pid_t pid;
    char buff[1000];

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

        i = 0;
        while (true) {
            i %= n;
            //sprintf(buff, "kill -USR1 %d", pid);
            //system(buff);
            kill(pid, SIGUSR1);
            sleep(t);
        }

    } else {
        son();
        exit(0);
    }

    waitpid(pid, (int*)0, 0);
    return 0;
}