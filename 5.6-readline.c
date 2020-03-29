#include <stdbool.h>
#include <stdio.h>

#include "lib/readline.c"

#define N 8

int main() {
    char buf[N] = {0};

    while (readline(buf, N)) {
        printf("%s", buf);
    }

    return 0;
}
