#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <dirent.h>

#define EODIR INT_MAX

#define ERR_ARGS "myls usage: ./myls path or ./mylss -r path"
#define ERR_ODIR "myls: unable to open"

int listContent(const char* path, bool isRecursive);
void recursiveReadR(const char* path, int* count, bool isRecursive);
bool isFile(struct dirent* currDir);
void printEntry(const char* path, struct dirent* currRead, char fileType);

int main(int argc, char** argv) {
    bool isRecursive;
    int count;

    if (argc < 2 || (argc == 2 && argv[1][1] == 'r')) {
        fprintf(stderr, "%s\n", ERR_ARGS);
        return -1;
    }

    isRecursive = (argc == 3) && (strcmp(argv[1], "-r") == 0);

    count = listContent(argv[(isRecursive) ? 2 : 1], isRecursive);
    
    if (count == EODIR) {
        fprintf(stderr, "%s %s\n", ERR_ODIR, argv[(isRecursive) ? 2 : 1]);
        return -2;
    }
    
    printf("\nTotal count: %d\n", count);

    return 0;
}

int listContent(const char* path, bool isRecursive) {
    char* rightPath;
    bool allocated;
    int count = 0;
    DIR* testOpenDir;
    
    if ((testOpenDir = opendir(path)) == NULL) {
        return EODIR;
    }

    closedir(testOpenDir);

    rightPath = path;
    allocated = false;

    if (path[strlen(path) - 1] != '/') {
        rightPath = malloc((strlen(path) + 2) * sizeof(char));
        sprintf(rightPath, "%s/", path);
        allocated = true;
    }

    recursiveReadR(rightPath, &count, isRecursive);

    if (allocated) {
        free(rightPath);
    }

    return count;
}

void recursiveReadR(const char* path, int* count, bool isRecursive) {
    char* buf;
    struct dirent* currRead;
    DIR* dir;

    buf = malloc((strlen(path) + 1) * sizeof(char));
    dir = opendir(path);

    if (dir == NULL) {
        printEntry(path, NULL, 'u');
        return;
    }

    while (currRead = readdir(dir)) {
        
        if (currRead->d_name[0] != '.') {
            printEntry(path, currRead, (isFile(currRead)) ? 'f' : 'd');

            if (isRecursive && !isFile(currRead)) {
                sprintf(buf, "%s%s/", path, currRead->d_name);
                recursiveReadR(buf, count, isRecursive);
            }

            (*count)++;
        }
    } 

    free(buf);
}

bool isFile(struct dirent* currDir) {
    return currDir->d_type == DT_REG;
}

void printEntry(const char* path, struct dirent* currRead, char fileType) {
    printf("%c: %s%s\n", fileType, path, (currRead) ? currRead->d_name : "");
}