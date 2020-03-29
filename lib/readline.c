#include <stdio.h>

int readline(char buf[], int n, FILE *stream) {
    int i = 0;

    while (i < n - 1) {
        char c = fgetc(stream);

        if (c == EOF) {
            buf[i] = '\0';
            return i;
        }

        buf[i++] = c;

        if (c == '\n') {
            buf[i] = '\0';
            return i;
        }
    }

    buf[i] = '\0';
    return -i;
}
