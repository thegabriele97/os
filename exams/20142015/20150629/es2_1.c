#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main (int argc, char *argv[]) {
    char buff[0x400];

    do {
        fgets(buff, 0x400, stdin);
        wait(NULL);

        if (!strcmp(buff, "end")) {
            break;
        }

        if (!fork()) {
            system(buff);
            exit(0);
        }

    } while(true);

    return 0;
}