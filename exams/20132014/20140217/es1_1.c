#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>

void child(int index, int *vet) {
    int tmp;

    if (vet[index + 1] < vet[index]) {
        tmp = vet[index];
        vet[index] = vet[index + 1];
        vet[index + 1] = tmp;
    }

    if (!fork()) {
        printf("C %d - P %d\n", getpid(), getppid());
        printf("%d] %d\n", index, vet[index]);
        printf("%d] %d\n", index + 1, vet[index + 1]);
        
        exit(0);
    }

    wait(NULL);
}

int main() {
    int i, n, *vet;

    scanf("%d", &n);
    vet = malloc(n * sizeof *vet);

    for (i = 0; i < n; i++) {
        scanf("%d", &vet[i]);
    }

    for (i = 0; i < n - 1; i++) {
        if (!fork()) {
            child(i, vet);
            exit(0);
        }
    }

    while(wait(NULL) > 0);
    free(vet);
    return 0;
}