#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define CHILD1 0x0
#define CHILD2 0x1

pid_t _pids[2];

void _hdl_p_sigint(int sig) {
    int i;

    for (i = 0; i < 2; i++) {
        kill(_pids[i], SIGKILL);
    }

    raise(SIGKILL);
}

void _hdl_p_router(int sig) {
    bool is_child_1 = (sig == SIGUSR1);

    printf("PARENT PID %d is routing 0x%x to CHILD %d\n", getpid(), sig, is_child_1 ? 2 : 1);
    sleep(2);

    kill(_pids[is_child_1 ? CHILD2 : CHILD1], sig);
}

void _hdl_c1_sigusr2(int sig) {
    printf("SIGUSR2 received by Child 2\n");
    fflush(stdout);
}

void _hdl_c2_sigusr1(int sig) {
    printf("SIGUSR1 received by Child 1\n");
    fflush(stdout);

    sleep(1);
    kill(getppid(), SIGUSR2);
}

void child1() {
    signal(SIGUSR2, _hdl_c1_sigusr2);

    while (true) {
        sleep(1);
        printf("Child 1 PID %d sending SIGUSR1 to CHILD2.\n", getpid());
        kill(getppid(), SIGUSR1);
        pause(); 
    }
}

void child2() {
    signal(SIGUSR1, _hdl_c2_sigusr1);

    while (true) {
        pause();
    }
}

int main() {

    signal(SIGUSR1, _hdl_p_router);
    signal(SIGUSR2, _hdl_p_router);
    signal(SIGINT, _hdl_p_sigint);

    if (!(_pids[CHILD1] = fork())) {
        child1();
        exit(0);
    } else {
        if (!(_pids[CHILD2] = fork())) {
            child2();
            exit(0);
        }
    }

    while (true) {
        pause();
    }

    return 0;
}