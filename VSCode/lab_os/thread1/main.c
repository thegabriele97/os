#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>

#define CODE_END 0x1

typedef struct {
    int fd_in;
    int fd_out;
} fd_t;

void *thread_print(void *fd_in_arg) {
    int fd_in;
    char ch;

    fd_in = (int)((long int)fd_in_arg);

    while (read(fd_in, &ch, 0x1) > 0) {
        if (ch == CODE_END) {
            break;
        }

        write(STDOUT_FILENO, &ch, 0x1);
    }

    pthread_exit(NULL);
}

void *thread_convert(void *fd_arg) {
    char ch;
    fd_t *fd = (fd_t *)fd_arg;
    
    while (read(fd->fd_in, &ch, 0x1) > 0) {
        if (ch == CODE_END) {
            write(fd->fd_out, &ch, 0x1);
            break;
        }

        ch = toupper(ch);
        write(fd->fd_out, &ch, 0x1);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t tid;
    int _pipe_read[2], _pipe_write[2];
    fd_t fd_conv;
    char buf[1024], code;

    pipe(_pipe_read);
    pipe(_pipe_write);

    fd_conv.fd_in = _pipe_read[0];
    fd_conv.fd_out = _pipe_write[1];
    pthread_create(&tid, NULL, thread_convert, (void *)&fd_conv);
    pthread_create(&tid, NULL, thread_print, (void *)(long int)_pipe_write[0]);

    fgets(buf, 1024, stdin);
    write(_pipe_read[1], buf, (strlen(buf) + 1) * sizeof *buf);
    
    code = CODE_END;
    write(_pipe_read[1], &code, 0x1);

    pthread_exit(NULL);
}