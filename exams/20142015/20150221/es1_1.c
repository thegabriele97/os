#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main (int argc, char *argv[]) {
    int a, n, i;
    char buff[0x400];

    if (argc != 3) {
        fprintf(stderr, "no args\n");
        return 1;
    }

    a = atoi(argv[1]);
    n = atoi(argv[2]);

    printf("(a=%d) C %d - P %d\n", a, getpid(), getppid());
    fflush(stdout);

    for (i = 0; a > 0 && i < n; i++) {
        if (!fork()) {
            sprintf(buff, "%d", a - 1);
            execl(argv[0], argv[0], buff, argv[2], NULL);
        }
    }

    while(wait(NULL) > 0);
    return 0;
}