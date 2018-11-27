#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "myAlarm.h"

void HDL_SIGALRM(int sig) {
    printf("Alarm ended!\n");
}

int main (int argc, char **argv) {
    int sec;

    signal(SIGALRM, HDL_SIGALRM);

    scanf("%d", &sec);
    ___alarm(sec);
    pause();

    return 0;
}