#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define BUFSIZE 2

bool startSon1 = false;

void HDL_SIGUSR1(int sig) {
    startSon1 = true;
}

void showFile(const char* fileName) {
    char buf[BUFSIZE];
    int fd, nR, nTot = 0;

    fd = open(fileName, O_RDONLY|O_SYNC);

    while (nTot < 50 && (nR = read(fd, buf, BUFSIZE)) > 0) {
        write(STDOUT_FILENO, buf, nR);
        nTot += nR;
    }

    printf("\n");
    close(fd);
}

void son1(const char* fileName) {

    signal(SIGUSR1, HDL_SIGUSR1);

    while (!startSon1) {
        pause();
    }

    printf("\n[%s]\n", fileName);
    showFile(fileName);
}

void son2(const char* fileName, pid_t son1PID) {

    printf("\n[%s]\n", fileName);
    showFile(fileName);

    kill(son1PID, SIGUSR1);
    sleep(5);
}

int main() {
    pid_t son1PID;

    if (son1PID = fork()) {
        if (!fork()) {
            son2("son2.txt", son1PID);
        }
    } else {
        son1("son1.txt");
    }

    while(wait((int *)0) > 0);

    return 0;
}