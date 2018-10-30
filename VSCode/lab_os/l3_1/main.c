#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSIZE 4098

typedef struct dirent* dir_entry;

void merge(const char* _src1, const char* _src2, const char* _dest, bool _verbose);
void recursive(const char* _src1, const char* _src2, const char* _dest, bool __v);
bool isInside(const char* path, dir_entry entry);
bool isFile(dir_entry currDir);
char* fixPath(const char* path);

int main(int argc, char** argv) {
    DIR* dest;
    dir_entry e;
    bool isVerbose;

    if (argc < 4) {
        fprintf(stderr, "merge: invalid arguments.\n");
    }

    if ((dest = opendir(argv[argc - 1])) == NULL) {
        fprintf(stderr, "merge: unable to open destination directory %s\n", argv[argc - 1]);
        exit(-1);
    } else if ((dest = opendir(argv[argc - 2])) == NULL) {
        fprintf(stderr, "merge: unable to open source directory %s\n", argv[argc - 2]);
        exit(-1);
    } else if ((dest = opendir(argv[argc - 3])) == NULL) {
        fprintf(stderr, "merge: unable to open source directory %s\n", argv[argc - 3]);
        exit(-1);
    }

    dest = opendir(argv[argc - 1]);
    while (e = readdir(dest)) {

        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0) {
            continue;
        }

        fprintf(stderr, "merge: destination directory must be void.\n");
        closedir(dest);
        exit(-1);
    }

    isVerbose = (argc == 5 && argv[1][1] == 'v');
    merge(argv[argc - 3], argv[argc - 2], argv[argc - 1], isVerbose);

    return 0;
}

void merge(const char* _src1, const char* _src2, const char* _dest, bool _verbose) {
    char *src1, *src2, *dest;

    src1 = fixPath(_src1);
    src2 = fixPath(_src2);
    dest = fixPath(_dest);

    recursive(src1, src2, dest, _verbose);
    recursive(src2, src1, dest, _verbose);

    free(src1);
    free(src2);
    free(dest);
}

void recursive(const char* _src1, const char* _src2, const char* _dest, bool __v) {
    char *src1, *src2, *dest, buff[BUFSIZ], del;
    int fdIn, fdOut, nR, sel;
    DIR *dir1;
    dir_entry entry;

    src1 = malloc(2 * (strlen(_src1) + 1) * sizeof(char));
    src2 = malloc(2 * (strlen(_src2) + 1) * sizeof(char));
    dest = malloc(2 * (strlen(_dest) + 1) * sizeof(char));

    dir1 = opendir(_src1);

    while (dir1 && (entry = readdir(dir1))) {

        if (entry->d_name[0] == '.') {
            continue;
        }

        del = (isFile(entry)) ? '\0' : '/';
        sprintf(src1, "%s%s%c", _src1, entry->d_name, del);
        sprintf(src2, "%s%s%c", _src2, entry->d_name, del);
        sprintf(dest, "%s%s%c", _dest, entry->d_name, del);

        if (!isFile(entry)) {

            if (__v) {
                printf("Creating %s\n", dest);
            }

            mkdir(dest, S_IRUSR|S_IWUSR|S_IXUSR);
            recursive(src1, src2, dest, __v);
            recursive(src2, src1, dest, __v);
        } else {

            if ((fdOut = open(dest, O_RDONLY|O_SYNC)) > 0) {
                close(fdOut);
                continue;
            }

            fdIn = open(src1, O_RDONLY|O_SYNC);
            fdOut = creat(dest, S_IRUSR|S_IWUSR);

            if (isInside(_src2, entry)) {
                printf("\nWhich file do you want to copy?\n");
                printf("1) %s\n", src1);
                printf("2) %s\n", src2);
                printf("> ");

                scanf("%d", &sel);
                printf("\n");

                if (sel == 2) {
                    close(fdIn);
                    fdIn = open(src2, O_RDONLY|O_SYNC);
                }
            }

            while ((nR = read(fdIn, buff, BUFSIZE)) > 0) {
                write(fdOut, buff, nR);
            }

            if (__v) {
                printf("Copying %s -> %s\n", (sel == 2) ? src2 : src1, dest);
            }

            close(fdIn);
            close(fdOut);
        }

    }

    free(src1);
    free(src2);
    free(dest);

    closedir(dir1);
}

bool isInside(const char* path, dir_entry entry) {
    DIR* dir;
    dir_entry e;

    dir = opendir(path);

    while (dir && (e = readdir(dir))) {
        if (strcmp(e->d_name, entry->d_name) == 0) {
            return true;
        }
    }

    return false;
}

bool isFile(dir_entry currDir) {
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