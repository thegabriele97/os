#include "myAlarm.h"
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

unsigned int ___alarm(unsigned int secs) {
    int i;

    if (!fork()) {

        for (i = 0; i < secs; i++) {
            sleep(1);
        }

        kill(getppid(), SIGALRM);
        exit(0);
    }

    return 0;
}