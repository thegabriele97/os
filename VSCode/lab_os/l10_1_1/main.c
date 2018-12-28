#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>

#define SIZ 101

enum {sem_read, sem_convert, sem_print};

typedef struct {
    char *buff;
    char *fileIn;
    char *fileOut;
    sem_t *sems;
} args_t;

void *_th_read(void *args) {
    FILE *fin;
    bool _____exit = false;
    args_t *arg = (args_t *)args;

    fin = fopen(arg->fileIn, "r");

    while (!_____exit) {
        sem_wait(&arg->sems[sem_read]);

        if (_____exit = fgets(arg->buff, SIZ - 1, fin) == NULL) {
            arg->buff[0] = '\0';
        }

        sem_post(&arg->sems[sem_convert]);
    }

    fclose(fin);
    pthread_exit(NULL);
}

void *_th_convert(void *args) {
    int i;
    char tmp;
    bool _____exit = false;
    args_t *arg = (args_t *)args;

    while (!_____exit) {
        sem_wait(&arg->sems[sem_convert]);

        if (!(_____exit = arg->buff[0] == '\0')) {
            for (i = 0; i < (strlen(arg->buff) / 2); i++) {
                tmp = arg->buff[i];
                arg->buff[i] = toupper(arg->buff[strlen(arg->buff) - 1 - i]);
                arg->buff[strlen(arg->buff) - 1 - i] = toupper(tmp);
            }
        }

        sem_post(&arg->sems[sem_print]);
    }

    pthread_exit(NULL);
}

void *_th_print(void *args) {
    FILE* fout;
    bool _____exit = false;
    args_t *arg = (args_t *)args;

    fout = fopen(arg->fileOut, "w");

    while (!_____exit) {
        sem_wait(&arg->sems[sem_print]);

        _____exit = (arg->buff[0] == '\0');
        fprintf(fout, "%s", arg->buff);

        sem_post(&arg->sems[sem_read]);
    }

    fclose(fout);
    pthread_exit(NULL);
}

int main (int argc, char *argv[]) {
    char buff[SIZ];
    pthread_t tid_read, tid_convert, tid_print;
    args_t args;
    sem_t sems[3];

    if (argc != 3) {
        return 1;
    }

    args.buff = buff;
    args.fileIn = argv[1];
    args.fileOut = argv[2];
    args.sems = sems;

    sem_init(&args.sems[sem_read], 0x0, 0x1);
    sem_init(&args.sems[sem_convert], 0x0, 0x0);
    sem_init(&args.sems[sem_print], 0x0, 0x0);

    pthread_create(&tid_read, NULL, _th_read, (void *)&args);
    pthread_create(&tid_convert, NULL, _th_convert, (void *)&args);
    pthread_create(&tid_print, NULL, _th_print, (void *)&args);

    pthread_join(tid_read, NULL);
    pthread_join(tid_convert, NULL);
    pthread_join(tid_print, NULL);

    return 0;
}