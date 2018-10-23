#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FILE_PATH_DIM 4096

#define ERR_O_FILE_STR "ERROR: unable to open"

int main(int argc, char** argv) {
    int fd, fd2, nR;
    char file_path[FILE_PATH_DIM], buf;

    if (argc <= 1) {
        printf("File to open: ");
        scanf("%s\n", file_path);
    } else {
        sprintf(file_path, argv[1]);
    }
    
    fd = open(file_path, O_RDONLY|O_SYNC);
    fd2 = open(file_path, O_RDONLY|O_SYNC);

    if (fd < 0) {
        fprintf(stderr, "%s %s\n", ERR_O_FILE_STR, file_path);
        return -1;
    }

    while ((nR = read(fd, &buf, sizeof(char))) > 0) {
        write(STDOUT_FILENO, &buf, nR);
    }

    return 0;
}