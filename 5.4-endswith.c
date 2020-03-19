#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void swap(char *x, char *y) {
    char tmp = *x;
    *x = *y;
    *y = tmp;
}

char *strend(char *s) {
    while (*s) {
        s++;
    }
    return --s;
}

void reverse(char *s) {
    char *e = strend(s);
    while (s < e) {
        swap(s++, e--);
    }
}

bool startswith(char *string, char *prefix) {
    while (*prefix) {
        if (*string++ != *prefix++) {
            return false;
        }
    }
    return true;
}

bool endswith(char *string, char *suffix) {
    reverse(string);
    reverse(suffix);
    return startswith(string, suffix);
}

int main() {
    char string[] = "01234567";
    char suffix[] = "567";
    assert(endswith(string, suffix));
    assert(!endswith(suffix, string));
    return 0;
}
