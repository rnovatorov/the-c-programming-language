#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/readline.h"

void reverse(char line[], int len) {
    char tmp;
    int left, right;

    for (left = 0, right = len - 1; left < right; left++, right--) {
        tmp = line[left];
        line[left] = line[right];
        line[right] = tmp;
    }
}

#define MAXLEN 6

int main() {
    int len;
    char line[MAXLEN];

    while ((len = readline(line, MAXLEN, stdin)) > 0) {
        reverse(line, len);
        printf("%s\n", line);
    }

    return 0;
}
