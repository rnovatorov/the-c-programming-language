#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse(char from[], char to[], int len) {
    int i;

    for (i = 0; i < len; i++) {
        to[i] = from[len - i - 1];
    }

    to[i] = '\0';
}

int readline(char buf[], int maxlen) {
    int i = 0;
    char c;

    while (i < maxlen && (c = getchar()) != EOF) {
        if (c == '\n') {
            buf[i] = '\0';
            return i;
        }
        buf[i++] = c;
    }

    return -i;
}

int main() {
    int len;
    int maxlen = 4;
    char line[maxlen], reversed[maxlen];

    while ((len = readline(line, maxlen)) > 0) {
        reverse(line, reversed, len);
        printf("%s\n", reversed);
    }

    return 0;
}
