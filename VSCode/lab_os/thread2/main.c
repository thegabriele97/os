#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFSIZ 0x400

typedef struct {
    char file_in[BUFFSIZ];
    char file_out[BUFFSIZ];
} file_t;

typedef struct {
    int *arr;
    int n;
} arr_t;

void *thread_sort(void *arg) {
    arr_t *array = (arr_t *)arg;
    int i, tmp, j;

    for (i = 1; i < array->n; i++) {
        tmp = array->arr[j = i];

        while (--j >= 0 && array->arr[j] > tmp) {
            array->arr[j + 1] = array->arr[j];
            array->arr[j] = tmp;
        }
    }

    pthread_exit(NULL);
}

void *thread_read_sort_write(void *arg) {
    pthread_t tid;
    file_t *files = (file_t *)arg;
    arr_t array;
    FILE *fp;
    int i;

    fp = fopen(files->file_in, "r");

    fscanf(fp, "%d\n", &array.n);
    array.arr = malloc(array.n * sizeof *array.arr);

    for (i = 0; i < array.n; i++) {
        fscanf(fp, "%d", &array.arr[i]);
    }

    pthread_create(&tid, NULL, thread_sort, (void *)&array);
    fclose(fp);

    fp = fopen(files->file_out, "w");

    fprintf(fp, "%d\n", array.n);
    pthread_join(tid, NULL);

    for (i = 0; i < array.n; i++) {
        fprintf(fp, "%d\n", array.arr[i]);
    }

    fclose(fp);
    free(array.arr);
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    int n, i;
    pthread_t *_tid_array;
    file_t files;
    
    if (argc < 3) {
        return 0x1;
    }
    
    n = atoi(argv[1]);
    _tid_array = malloc(n * sizeof *_tid_array);

    for (i = 0; i < n; i++) {
        sprintf(files.file_in, "%s%d", argv[2], i);
        sprintf(files.file_out, "%s%d", argv[3], i);

        pthread_create(&_tid_array[i], NULL, thread_read_sort_write, (void *)&files);
    }

    for (i = 0; i < n; i++) {
        pthread_join(_tid_array[i], NULL);
    }

    free(_tid_array);
    return 0;
}