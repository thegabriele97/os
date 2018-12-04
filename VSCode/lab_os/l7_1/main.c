#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFSIZ 0x1000

void child(const char* file_name) {
    FILE* fp;
    char buf[BUFFSIZ];
    
    fp = fopen(file_name, "r");

    fprintf(stdout, "\n\n\t\t\t[%s]\n", file_name);

    while (fgets(buf, BUFFSIZ - 1, fp) != NULL) {
        fprintf(stdout, "%s", buf);
    }

    fclose(fp);
}

int main() {
    int i;
    char *file_name[2] = {"test_1.txt", "test_2.txt"};

    for (i = 0; i < 2; i++) {

        if (!fork()) {
            child(file_name[i]);
            exit(0);
        }

        wait((int *)0);
    }

    printf("\n\nDone!\n");

    return 0;
}