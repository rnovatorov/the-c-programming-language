#include <stdbool.h>
#include <stdio.h>

#define N 8

int readline(char buf[], int n) {
    int i = 0;

    while (i < n - 1) {
        char c = getchar();

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

int main() {
    char buf[N] = {0};

    while (readline(buf, N)) {
        printf("%s", buf);
    }

    return 0;
}
