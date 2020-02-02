#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

const int BUFSIZE = 2;

struct List {
    struct List *next;
    char *buf;
};

void unwrap(struct List *first) {
    while (true) {
        assert(first != NULL);

        for (int i = 0; i < BUFSIZE; i++) {
            char c = first->buf[i];
            if (c == EOF) {
                return;
            }
            putchar(c);
        }

        first = first->next;
    }
}

int longest(struct List *first, struct List *last, int cur, int max) {
    struct List list;

    if (first == NULL) {
        assert(last == NULL);
        assert(cur == 0);
        last = first = &list;
    } else {
        assert(last != NULL);
        assert(cur != 0);
        last = last->next = &list;
    }

    char buf[BUFSIZE];
    list.buf = buf;

    for (int i = 0; i < BUFSIZE; i++) {
        char c = getchar();
        if (c == EOF) {
            if (cur > max) {
                buf[i] = c;
                putchar('\r');
                unwrap(first);
            }
            return -1;
        }
        if (c == '\n') {
            if (cur > max) {
                max = cur;
                buf[i] = EOF;
                putchar('\r');
                unwrap(first);
            }
            return max;
        }
        buf[i] = c;
        cur++;
    }

    return longest(first, last, cur, max);
}

int main() {
    int max = 0;

    while (max != -1) {
        max = longest(NULL, NULL, 0, max);
    }
    putchar('\n');

    return 0;
}
