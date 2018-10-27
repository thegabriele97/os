#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE 4096

#define EODIR INT_MAX
#define EEDEST (EODIR - 1)

#define ERR_NO_ARGS "mycpr usage: ./mycpr source dest"
#define ERR_NO_SRC "mycpr: unable to open"

typedef int error;

int copyDirectory(const char* src, const char* dest);
void recursiveR(const char* src, const char* dest);
bool isFile(struct dirent* currDir);
char* fixPath(const char* path);

int main(int argc, char** argv) {
    error e;
    
    if (argc < 2) {
        fprintf(stderr, "%s\n", ERR_NO_ARGS);
    }

    e = copyDirectory(argv[1], argv[2]);

    if (e == EODIR) {
        fprintf(stderr, "%s %s\n", ERR_NO_SRC, argv[1]);
    } else if (e == EEDEST) {
        fprintf(stderr, "mycpr: %s already exists\n", argv[2]);
    }
    
    return 0;
}

int copyDirectory(const char* src, const char* dest) {
    char *rightSrc, *rightDest;
    DIR* testOpenDir;
    
    if ((testOpenDir = opendir(src)) == NULL) {
        return EODIR;
    }

    closedir(testOpenDir);

    if ((testOpenDir == opendir(dest)) != NULL) {
        closedir(testOpenDir);
        return EEDEST;
    }

    rightSrc = fixPath(src);
    rightDest = fixPath(dest);
    mkdir(rightDest, S_IRUSR|S_IXUSR|S_IWUSR);
    recursiveR(rightSrc, rightDest);

    free(rightSrc);
    free(rightDest);
    return 0;
}

void recursiveR(const char* src, const char* dest) {
    char *buf, *buf2, del;
    char cpBuffer[BUFSIZE];
    int fdIn, fdOut, nR;
    struct dirent* currRead;
    DIR* dir;

    buf = malloc(2 * (strlen(src) + 1) * sizeof(char));
    buf2 = malloc(2 * (strlen(dest) + 1) * sizeof(char));
    dir = opendir(src);

    if (dir == NULL) {
        return;
    }

    while (currRead = readdir(dir)) {
        
        if (currRead->d_name[0] != '.') {

            del = (isFile(currRead)) ? '\0' : '/';
            sprintf(buf, "%s%s%c", src, currRead->d_name, del);
            sprintf(buf2, "%s%s%c", dest, currRead->d_name, del);

            if (!isFile(currRead)) {
                mkdir(buf2, S_IRUSR|S_IXUSR|S_IWUSR);
                recursiveR(buf, buf2);
            } else {
                fdIn = open(buf, O_RDONLY|O_SYNC);
                fdOut = creat(buf2, S_IRUSR|S_IWUSR);

                while ((nR = read(fdIn, cpBuffer, BUFSIZE)) > 0) {
                    write(fdOut, cpBuffer, nR);
                }

                close(fdIn);
                close(fdOut);
            }
        }
    } 

    closedir(dir);
    free(buf);
}

bool isFile(struct dirent* currDir) {
    return currDir->d_type == DT_REG;
}

char* fixPath(const char* path) {
    char* rightPath;

    rightPath = malloc((strlen(path) + 1) * sizeof(char));
    strcpy(rightPath, path);

    if (path[strlen(path) - 1] != '/') {
        rightPath = malloc((strlen(path) + 2) * sizeof(char));
        sprintf(rightPath, "%s/", path);
    }

    return rightPath;
}