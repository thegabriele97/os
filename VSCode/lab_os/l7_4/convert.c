#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

int main(int argc, char** argv) {
    char ch;

    while (read(STDIN_FILENO, &ch, 0x1) > 0) {

        if (ch == argv[1][0]) {
            break;
        }

        ch = toupper(ch);
        write(STDOUT_FILENO, &ch, 0x1);
    }

    return 0;
}