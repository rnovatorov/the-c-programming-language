#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse(char line[], int len) {
    char tmp;
    int left, right;

    for (left = 0, right = len - 1; left < right; left++, right--) {
        tmp = line[left];
        line[left] = line[right];
        line[right] = tmp;
    }
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

    return -1;
}

int main() {
    int len;
    int maxlen = 4;
    char line[maxlen];

    while ((len = readline(line, maxlen)) > 0) {
        reverse(line, len);
        printf("%s\n", line);
    }

    return 0;
}
