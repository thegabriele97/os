#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main (int argc, char *argv[]) {
    int pp[2], pp2[2], n;
    
    pipe(pp);

    if (!fork()) {
        pipe(pp2);

        if (!fork()) {
            scanf("%d", &n);
            write(pp2[1], &n, sizeof(int));
            exit(0);
        }

        read(pp2[0], &n, sizeof(int));
        n*=-1;
        write(pp[1], &n, sizeof(int));

        exit(0);
    }

    read(pp[0], &n, sizeof(int));
    printf("%d", n);

    exit(0);
}