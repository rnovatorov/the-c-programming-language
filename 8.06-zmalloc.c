#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/zmalloc.h"

int main() {
    char *s = zmalloc(6);
    assert(s);
    s[0] = 'h';
    s[1] = 'e';
    s[2] = 'l';
    s[3] = 'l';
    s[4] = 'o';
    s[5] = '\0';
    printf("%s ", s);

    char *t = zmalloc(6);
    assert(t);
    t[0] = 'w';
    t[1] = 'o';
    t[2] = 'r';
    t[3] = 'l';
    t[4] = 'd';
    t[5] = '\0';
    printf("%s\n", t);

    zfree(s);
    zfree(t);
    return EXIT_SUCCESS;
}
