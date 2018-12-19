#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

typedef struct {
    sem_t *_sem;
    pthread_t *_th;
} args_t;

#define SEM(_semIndex) (((args_t *)args)->_sem[_semIndex])
#define TH(_thIndex) (((args_t *)args)->_th[_thIndex])

void *thA(void *args);
void *thB(void *args);
void *thC(void *args);
void *thD(void *args);
void *thE(void *args);
void *thF(void *args);
void *thG(void *args);
void *thH(void *args);
void *thI(void *args);

enum sem_e {
    semA, semB, semC, semD, semE, semF, semG, semH, semI
};

enum Th_e {
    ThA, ThB, ThC, ThD, ThE, ThF, ThG, ThH, ThI
};

void *thA(void *args) {

    sem_wait(&SEM(semA));

    printf("A\n");
    sleep(1);

    pthread_create(&TH(ThB), NULL, thB, args);
    pthread_create(&TH(ThC), NULL, thC, args);
    pthread_create(&TH(ThD), NULL, thD, args);

    sem_post(&SEM(semB));
    sem_post(&SEM(semD));
    sem_post(&SEM(semC));

    pthread_exit(NULL);
}

void *thB(void *args) {

    sem_wait(&SEM(semB));

    printf("B\n");
    sleep(5);

    pthread_create(&TH(ThI), NULL, thI, args);
    sem_post(&SEM(semI));

    pthread_exit(NULL);
}

void *thC(void *args) {

    sem_wait(&SEM(semC));

    printf("C\n");
    sleep(1);

    pthread_create(&TH(ThE), NULL, thE, args);
    pthread_create(&TH(ThF), NULL, thF, args);

    sem_post(&SEM(semE));
    sem_post(&SEM(semF));

    pthread_exit(NULL);
}

void *thD(void *args) {

    sem_wait(&SEM(semD));

    printf("D\n");
    sleep(2);

    pthread_create(&TH(ThH), NULL, thH, args);
    sem_post(&SEM(semH));

    pthread_exit(NULL);
}

void *thE(void *args) {

    sem_wait(&SEM(semE));

    printf("E\n");
    sleep(3);

    pthread_create(&TH(ThG), NULL, thG, args);
    sem_post(&SEM(semG));

    pthread_exit(NULL);
}

void *thF(void *args) {

    sem_wait(&SEM(semF));

    printf("F\n");
    sleep(1);

    sem_post(&SEM(semG));

    pthread_exit(NULL);
}

void *thG(void *args) {

    sem_wait(&SEM(semG));
    sem_wait(&SEM(semG));

    printf("G\n");
    sleep(1);

    sem_post(&SEM(semI));

    pthread_exit(NULL);
}

void *thH(void *args) {

    sem_wait(&SEM(semH));

    printf("H\n");
    sleep(1);

    sem_post(&SEM(semI));

    pthread_exit(NULL);
}

void *thI(void *args) {

    sem_wait(&SEM(semI));
    sem_wait(&SEM(semI));
    sem_wait(&SEM(semI));

    printf("I\n");
    sleep(1);

    pthread_create(&TH(ThA), NULL, thA, args);
    sem_post(&SEM(semA));

    pthread_exit(NULL);
}

int main () {
    sem_t _sem[0x9];
    pthread_t _th[0x9];
    args_t args;

    sem_init(&_sem[semA], 0x0, 0x0);
    sem_init(&_sem[semB], 0x0, 0x0);
    sem_init(&_sem[semC], 0x0, 0x0);
    sem_init(&_sem[semD], 0x0, 0x0);
    sem_init(&_sem[semE], 0x0, 0x0);
    sem_init(&_sem[semF], 0x0, 0x0);
    sem_init(&_sem[semG], 0x0, 0x0);
    sem_init(&_sem[semH], 0x0, 0x0);
    sem_init(&_sem[semI], 0x0, 0x0);
    
    args._sem = _sem;
    args._th = _th;

    pthread_create(&_th[ThA], NULL, thA, (void *)&args);
    sem_post(&_sem[semA]);

    pthread_exit(NULL);
}