#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <signal.h>
#include <pthread.h>

typedef struct {
    int N;
    int i;
} index_t;

void print_thread_id(pthread_t id) {
    size_t i;
    for (i = sizeof(i); i; --i)
        printf("%02x", *(((unsigned char*) &id) + i - 1));
}

void *_th(void *args) {
    index_t index_or = *(index_t *)args;
    index_t index = index_or;
    pthread_t tids[2];

    if (index_or.i == index_or.N) {
        print_thread_id(pthread_self());
        printf("\n");
        pthread_exit(NULL);
    }

    index.i++;
    pthread_create(tids, NULL, _th, (void *)&index);

    if (index_or.i % 2 != 0) {
        pthread_create(tids + 1, NULL, _th, (void *)&index);
        pthread_join(tids[1], NULL);
    }

    pthread_join(*tids, NULL);
    pthread_exit(NULL);
}


int main(int argc, char **argv) {
    index_t index;
    pthread_t tid;

    if (argc != 2) {
        return 1;
    }

    index.N = atoi(argv[1]);
    index.i = 1;

    pthread_create(&tid, NULL, _th, (void *)&index);
    pthread_join(tid, NULL);

    return 0;
}