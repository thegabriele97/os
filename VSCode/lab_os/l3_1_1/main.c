#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define N 6

int main(int argc, char** argv) {
    pid_t pid0, pid1;
    int n, t, i, j = 0;
    char *buf, *buf1;
    
    buf = malloc((strlen(argv[1]) + strlen(argv[2]) + 3) * sizeof *buf);
    buf1 = malloc((strlen(argv[1]) + strlen(argv[2]) + 3) * sizeof *buf);
    n = atoi(argv[1]);
    t = atoi(argv[2]);

    if (!n) {
        sleep(t);
        printf("Process with PID %d terminated after %d seconds.\n", getpid(), t);
        return 0;
    }

    sprintf(buf, "%d", --n);
    sprintf(buf1, "%d", t);

    for (i = 0; i < 2; i++) {
        if (!(pid0 = fork())) {
            execl(argv[0], argv[0], buf, buf1, NULL);
        } else {
            printf("Generating process PID %d from PID %d.\n", pid0, getpid());
        }
    }

    free(buf);
    free(buf1);
    return 0;
}