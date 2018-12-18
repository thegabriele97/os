#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

typedef enum code_e {
    CODE_GO
} code_t;

void childB();
void childC();
void childD();
void childE();
void childF();
void childG();
void childH();
void childI();

int ppA[2], ppB[2], ppC[2], ppD[2], ppE[2], ppF[2], ppG[2], ppH[2], ppI[2];

void childB() {
    code_t code;

    if (!fork()) {
        childI();
        exit(0);
    }

    while (true) {

        read(ppB[0], &code, sizeof(code_t));

        printf("B\n");
        sleep(5);

        write(ppI[1], &code, sizeof(code_t));
    }
}

void childC() {
    code_t code;

    pipe(ppE);
    pipe(ppF);
    pipe(ppG);

    if (!fork()) {
        childE();
        exit(0);
    } else if (!fork()) {
        childF();
        exit(0);
    }

    while (true) {
        
        read(ppC[0], &code, sizeof(code_t));

        printf("C\n");
        sleep(1);

        write(ppE[1], &code, sizeof(code_t));
        write(ppF[1], &code, sizeof(code_t));
    }
}

void childD() {
    code_t code;

    pipe(ppH);

    if (!fork()) {
        childH();
        exit(0);
    }
    
    while (true) {

        read(ppD[0], &code, sizeof(code_t));

        printf("D\n");
        sleep(2);

        write(ppH[1], &code, sizeof(code_t));
    }
}

void childE() {
    code_t code;

    if (!fork()) {
        childG();
        exit(0);
    }

    while (true) {

        read(ppE[0], &code, sizeof(code_t));

        printf("E\n");
        sleep(3);

        write(ppG[1], &code, sizeof(code_t));
    }
}

void childF() {
    code_t code;

    while(true) {

        read(ppF[0], &code, sizeof(code_t));

        printf("F\n");
        sleep(1);

        write(ppG[1], &code, sizeof(code_t));
    }
}

void childG() {
    code_t code;

    while (true) {

        read(ppG[0], &code, sizeof(code_t));
        read(ppG[0], &code, sizeof(code_t));

        printf("G\n");
        sleep(1);

        write(ppI[1], &code, sizeof(code_t));
    }
}

void childH() {
    code_t code;

    while (true) {

        read(ppH[0], &code, sizeof(code_t));

        printf("H\n");
        sleep(1);

        write(ppI[1], &code, sizeof(code_t));
    }
}

void childI() {
    code_t code;

    while (true) {

        read(ppI[0], &code, sizeof(code_t));
        read(ppI[0], &code, sizeof(code_t));
        read(ppI[0], &code, sizeof(code_t));
        
        printf("I\n");
        sleep(1);

        write(ppA[1], &code, sizeof(code_t));
    }
}

int main() {
    code_t code = CODE_GO;

    pipe(ppA);
    pipe(ppB);
    pipe(ppC);
    pipe(ppD);
    pipe(ppI);

    if (!fork()) {
        childB();
        exit(0);
    } else if (!fork()) {
        childC();
        exit(0);
    } else if (!fork()) {
        childD();
        exit(0);
    } 

    write(ppA[1], &code, sizeof(code_t));
    
    while (true) {

        read(ppA[0], &code, sizeof(code_t));

        printf("A\n");
        sleep(1);

        write(ppB[1], &code, sizeof(code_t));
        write(ppC[1], &code, sizeof(code_t));
        write(ppD[1], &code, sizeof(code_t));
    }

    while(wait(NULL) > 0);
    return 0;
}