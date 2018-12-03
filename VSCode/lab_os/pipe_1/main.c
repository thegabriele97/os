#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFSIZ 0x400
#define CODE_EXIT 0x0
#define CODE_CONFIRM_EXIT 0X1
#define CODE_READ 0x2
#define CODE_ACK 0x3
#define CODE_READY 0x4

bool _kill = false;

void _hdl_sigint(int sig) {
    _kill = true;
}

void parent(int p_fd_out, int c_fd_in) {
    char buf[BUFFSIZ];
    char code;
    bool ___exit = false;

    signal(SIGINT, SIG_DFL);

    while (read(c_fd_in, &code, 0x1) > 0) {
        if (code == CODE_READY) {
            break;
        }
    }

    while (!___exit) {

        if (!_kill) {
            printf("\nInput\t:\t");
            fflush(stdout);
            fgets(buf, BUFFSIZ, stdin);
            buf[strlen(buf) - 1] = '\0';

            code = CODE_READ;
            write(p_fd_out, &code, 0x1);
            write(p_fd_out, buf, (strlen(buf) + 1) * sizeof *buf);
        } else {
            buf[0] = CODE_EXIT;
            buf[1] = CODE_CONFIRM_EXIT;
            write(p_fd_out, buf, 0x2);
        }

        while (read(c_fd_in, &code, 0x1) > 0) {
            read(c_fd_in, buf, BUFFSIZ);

            if (code == CODE_ACK) {
                printf("Output\t:\t%s\n", buf);
                fflush(stdout);
            } else if (CODE_EXIT) {
                ___exit = true;
            }

            break;
        }
    }
}

void child(int p_fd_in, int c_fd_out) {
    char other_string[BUFFSIZ], buf[BUFFSIZ];
    char code;

    code = CODE_READY;
    printf("Other string is: ");
    scanf("%s", other_string);
    write(c_fd_out, &code, 0x1);

    while (read(p_fd_in, &code, 0x1) > 0) {
        read(p_fd_in, buf, BUFFSIZ);

        if (code == CODE_READ) {
            code = CODE_ACK;
            sleep(1);
            
            sprintf(buf, "%s%s", buf, other_string);
            write(c_fd_out, &code, 0x1);
        } else if (code == CODE_EXIT) {
            buf[0] = CODE_EXIT;
            buf[1] = CODE_CONFIRM_EXIT;
            buf[2] = '\0';
        }

        write(c_fd_out, buf, (strlen(buf) + 1) * sizeof *buf);
        
        if (buf[0] == CODE_EXIT) {
            printf("Exiting..\n");
            break;
        }
    }
}

int main() {
    int _pipe_p[2], _pipe_c[2];
    pid_t pid;

    pipe(_pipe_p);
    pipe(_pipe_c);

    if (pid = fork()) {
        close(_pipe_p[0]);
        close(_pipe_c[1]);

        parent(_pipe_p[1], _pipe_c[0]);

    } else {
        close(_pipe_c[0]);
        close(_pipe_p[1]);

        child(_pipe_p[0], _pipe_c[1]);
        exit(0);
    }

    waitpid(pid, (int *)0, 0);
    return 0;
}