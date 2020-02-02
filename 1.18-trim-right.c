#include <stdbool.h>
#include <stdio.h>

void trimSpaces() {
    int n = 0;

    while (true) {
        char c = getchar();
        switch (c) {
        case EOF:
            return;
        case ' ':
            n++;
            break;
        case '\n':
            n = 0;
        default:
            for (; n > 0; n--) {
                putchar(' ');
            }
            putchar(c);
        }
    }
}

int main() {
    trimSpaces();
    return 0;
}
