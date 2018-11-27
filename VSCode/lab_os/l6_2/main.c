#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <stdbool.h>

#define BUF 256

pid_t pid;

int main() {
    char buf[BUF];
    bool askForPID = true;

    while (true) {
        if (askForPID) {
            printf("PID to manage: ");
            scanf("%d", &pid);
            askForPID = false;
        }
        
        printf("Command to sent to %d: ", pid);
        scanf("%s", buf);

        if (strcmp(buf, "sum") == 0) {
            printf("Sending SIGUSR2 to PID %d...\n", pid);
            kill(pid, SIGUSR2);
        } else if (strcmp(buf, "difference") == 0) {
            printf("Sending SIGUSR1 to PID %d...\n", pid);
            kill(pid, SIGUSR1);
        } else if (strcmp(buf, "end") == 0) {
            printf("Sending SIGINT to PID %d...\n", pid);
            kill(pid, SIGINT);
            askForPID = true;
        }
    }

    return 0;
}