#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

int a, b;

void HDL_SIGUSR1(int sig) {
    printf("Received SIGUSR1: %d-%d=%d\n", a, b, a - b);
}

void HDL_SIGUSR2(int sig) {
    printf("Received SIGUSR1: %d+%d=%d\n", a, b, a + b);
}

int main() {

    signal(SIGUSR1, HDL_SIGUSR1);
    signal(SIGUSR2, HDL_SIGUSR2);

    scanf("%d%d", &a, &b);

    while (true) {
        printf("Waiting for a signal..\n");
        pause();
    }

    return 0;
}