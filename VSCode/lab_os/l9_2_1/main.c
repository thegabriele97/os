#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

enum {
    semA,
    semB,
    semC,
    semD, 
    semE,
    semF,
    semG,
    semH,
    semI
};

enum {
    ThA,
    ThB,
    ThC,
    ThD, 
    ThE,
    ThF,
    ThG,
    ThH,
    ThI
};

void *thA(void *args) {

    while(true) {
        sem_wait(&((sem_t *)args)[semA]);

        printf("A\n");
        sleep(1);

        sem_post(&((sem_t *)args)[semB]);
        sem_post(&((sem_t *)args)[semC]);
        sem_post(&((sem_t *)args)[semD]);
    }
}

void *thB(void *args) {

    while (true) {
        sem_wait(&((sem_t *)args)[semB]);

        printf("B\n");
        sleep(5);

        sem_post(&((sem_t *)args)[semI]);
    }
}

void *thC(void *args) {
    
    while (true) {
        sem_wait(&((sem_t *)args)[semC]);

        printf("C\n");
        sleep(1);

        sem_post(&((sem_t *)args)[semE]);
        sem_post(&((sem_t *)args)[semF]);
    }
}

void *thD(void *args) {
    
    while (true) {
        sem_wait(&((sem_t *)args)[semD]);

        printf("D\n");
        sleep(2);

        sem_post(&((sem_t *)args)[semH]);
    }
}

void *thE(void *args) {
    
    while (true) {
        sem_wait(&((sem_t *)args)[semE]);

        printf("E\n");
        sleep(3);

        sem_post(&((sem_t *)args)[semG]);
    }
}

void *thF(void *args) {
    
    while (true) {
        sem_wait(&((sem_t *)args)[semF]);

        printf("F\n");
        sleep(1);

        sem_post(&((sem_t *)args)[semG]);
    }
}

void *thG(void *args) {
    
    while (true) {
        sem_wait(&((sem_t *)args)[semG]);
        sem_wait(&((sem_t *)args)[semG]);

        printf("G\n");
        sleep(1);

        sem_post(&((sem_t *)args)[semI]);
    }
}

void *thH(void *args) {
    
    while (true) {
        sem_wait(&((sem_t *)args)[semH]);

        printf("H\n");
        sleep(1);

        sem_post(&((sem_t *)args)[semI]);
    }
}

void *thI(void *args) {
    
    while (true) {
        sem_wait(&((sem_t *)args)[semI]);
        sem_wait(&((sem_t *)args)[semI]);
        sem_wait(&((sem_t *)args)[semI]);

        printf("I\n");
        sleep(1);

        sem_post(&((sem_t *)args)[semA]);
    }
}

int main() {
    sem_t _sem[0x9];
    pthread_t _th[0x9];

    sem_init(&_sem[semA], 0, 1);
    sem_init(&_sem[semB], 0, 0);
    sem_init(&_sem[semC], 0, 0);
    sem_init(&_sem[semD], 0, 0);
    sem_init(&_sem[semE], 0, 0);
    sem_init(&_sem[semF], 0, 0);
    sem_init(&_sem[semG], 0, 0);
    sem_init(&_sem[semH], 0, 0);
    sem_init(&_sem[semI], 0, 0);

    pthread_create(&_th[ThA], NULL, thA, (void *)_sem);
    pthread_create(&_th[ThB], NULL, thB, (void *)_sem);
    pthread_create(&_th[ThC], NULL, thC, (void *)_sem);
    pthread_create(&_th[ThD], NULL, thD, (void *)_sem);
    pthread_create(&_th[ThE], NULL, thE, (void *)_sem);
    pthread_create(&_th[ThF], NULL, thF, (void *)_sem);
    pthread_create(&_th[ThG], NULL, thG, (void *)_sem);
    pthread_create(&_th[ThH], NULL, thH, (void *)_sem);
    pthread_create(&_th[ThI], NULL, thI, (void *)_sem);

    pthread_join(_th[ThA], NULL);
    pthread_join(_th[ThB], NULL);
    pthread_join(_th[ThC], NULL);
    pthread_join(_th[ThD], NULL);
    pthread_join(_th[ThE], NULL);
    pthread_join(_th[ThF], NULL);
    pthread_join(_th[ThG], NULL);
    pthread_join(_th[ThH], NULL);
    pthread_join(_th[ThI], NULL);

    return 0;
}