#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/dynarray.h"
#include "lib/heap.h"
#include "lib/readline.h"
#include "lib/strncopy.h"

#define MAXLEN 256
#define MAXLINES 2048

struct sorter {
    struct dynarray *q;
};

struct sorter *sorter_alloc() {
    struct sorter *s = malloc(sizeof(struct sorter));
    if (s == NULL) {
        return NULL;
    }

    s->q = dynarray_alloc(0);
    if (s->q == NULL) {
        free(s);
        return NULL;
    }

    return s;
}

void sorter_free(struct sorter *s) {
    assert(s != NULL);

    for (int i = 0; i < dynarray_len(s->q); i++) {
        free(dynarray_get(s->q, i));
    }
    dynarray_free(s->q);

    free(s);
}

bool sorter_push(struct sorter *s, char *buf, size_t len) {
    assert(s != NULL);

    char *line = malloc((len + 1) * sizeof(char *));
    if (line == NULL) {
        return false;
    }

    strncopy(line, buf, len);
    line[len] = '\0';

    bool ok = heap_push(s->q, line);
    if (!ok) {
        free(line);
        return false;
    }
    return true;
}

char *sorter_pop(struct sorter *s) {
    assert(s != NULL);

    if (!dynarray_len(s->q)) {
        return NULL;
    }
    return heap_pop(s->q);
}

int main() {
    char buf[MAXLEN];

    struct sorter *s = sorter_alloc();
    if (s == NULL) {
        fprintf(stderr, "error: failed to allocate sorter\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < MAXLINES; i++) {
        int nread = readline(buf, MAXLEN, stdin);
        if (nread < 0) {
            sorter_free(s);
            fprintf(stderr, "error: line too long\n");
            return EXIT_FAILURE;
        }

        if (nread > 0) {
            bool ok = sorter_push(s, buf, nread);
            if (!ok) {
                sorter_free(s);
                fprintf(stderr, "error: failed to push to sorter\n");
                return EXIT_FAILURE;
            }
            continue;
        }

        assert(!nread);
        char *line;
        while ((line = sorter_pop(s))) {
            printf("%s", line);
            free(line);
        }
        sorter_free(s);
        return EXIT_SUCCESS;
    }

    sorter_free(s);
    fprintf(stderr, "error: too many lines\n");
    return EXIT_FAILURE;
}
