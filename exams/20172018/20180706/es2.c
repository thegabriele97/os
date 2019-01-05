#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <signal.h>
#include <pthread.h>

bool can = true;

void _sig_sigalrm(int sig) {
    can ^= true;   
}

bool recur(int n) {

    printf("\r%d%c", n, '%');

    if (n == 30) {
        printf("\r%d%c", 100, '%');
        return true;
    }

    fflush(stdout);
    sleep(1);

    if (can) {
        return recur(n + 1);
    }

    return can;
}

int main(int argc, char **argv) {
    bool res;
    int sec;

    signal(SIGALRM, _sig_sigalrm);

    sec = atoi(argv[1]);
    alarm(sec);
    printf("Alarm setted at %ds\n", sec);

    res = recur(0);
    printf("\n0x%x\n", res);

    return 0;
}