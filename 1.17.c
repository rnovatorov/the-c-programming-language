#include <stdbool.h>
#include <stdio.h>

#define LONGLINE 80

void printBuf(char buf[], int len) {
    for (int i = 0; i < len; i++) {
        putchar(buf[i]);
    }
}

bool printLine() {
    while (true) {
        char c = getchar();
        if (c == EOF) {
            return true;
        }
        if (c == '\n') {
            putchar(c);
            return false;
        }
        putchar(c);
    }
}

bool printIfLongerThan(int longLine) {
    char buf[longLine];

    for (int i = 0; i < longLine; i++) {
        char c = getchar();
        if (c == EOF) {
            return true;
        }
        if (c == '\n') {
            return false;
        }
        buf[i] = c;
    }

    printBuf(buf, longLine);
    return printLine();
}

int main() {
    bool eof = false;
    while (!eof) {
        eof = printIfLongerThan(LONGLINE);
    }
    return 0;
}
