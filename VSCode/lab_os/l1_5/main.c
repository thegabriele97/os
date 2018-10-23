#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFSIZE 4098

#define ERR_NO_ARGS "mycp: to few arguments"
#define ERR_O_FILE "mycp: unable to open"
#define ERR_FILE_EXISTS "mycp: output file already exists"

typedef char byte;

int main(int argc, char** argv) {
    int fd1, fd2, nR;
    byte buf[BUFFSIZE];

    if (argc < 2) {
        fprintf(stderr, "%s\n", ERR_NO_ARGS);
        exit(-1);
    }

    if (open(argv[2], O_WRONLY) >= 0) {
        fprintf(stderr, "%s\n", ERR_FILE_EXISTS);
        exit(-2);
    }

    fd1 = open(argv[1], O_RDONLY|O_SYNC);

    if (fd1 < 0) {
        fprintf(stderr, "%s %s\n", ERR_O_FILE, argv[1]);
        exit(-2);
    }

    fd2 = creat(argv[2], S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);

    while ((nR = read(fd1, buf, BUFFSIZE)) > 0) {
        write(fd2, buf, nR);
    }

    close(fd1);
    close(fd2);

    return 0;
}