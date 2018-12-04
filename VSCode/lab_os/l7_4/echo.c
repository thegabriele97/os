#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFSIZ 0x400
#define CODE_END 0x1

//ECHO PROGRAM
int main() {
    int _pipe[2];
    char buf[BUFFSIZ];

    pipe(_pipe);

    if (!fork()) {
        close(_pipe[0]);

        fgets(buf, BUFFSIZ, stdin);
        write(_pipe[1], buf, (strlen(buf) + 1) * sizeof *buf);
        
        execlp("echo", "echo", buf, NULL);
        exit(0);
    }

    close(_pipe[1]);
    wait((int *)0);

    buf[0] = CODE_END;
    buf[1] = '\0';

    dup2(_pipe[0], STDIN_FILENO);
    execl("./convert", "./convert", buf, NULL);

    return 0;
}