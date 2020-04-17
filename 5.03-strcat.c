#include <stdio.h>

void cat(char *l, char *r) {
    while (*l) {
        l++;
    }
    while ((*l++ = *r++)) {
    }
}

int main() {
    char a[] = "hello\0_____";
    char b[] = " world";
    cat(a, b);

    printf("%s\n", a);
    return 0;
}
