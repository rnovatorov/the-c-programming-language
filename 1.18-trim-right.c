#include <stdbool.h>
#include <stdio.h>

void trimChar(char t) {
    int n = 0;
    char c;

    while ((c = getchar()) != EOF) {
        if (c == t) {
            n++;
        } else if (c == '\n') {
            n = 0;
        } else {
            for (; n > 0; n--) {
                putchar(t);
            }
            putchar(c);
        }
    }
}

int main() {
    trimChar(' ');
    return 0;
}
