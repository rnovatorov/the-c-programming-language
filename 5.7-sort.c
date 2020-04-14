#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/dynarray.h"
#include "lib/heapq.h"
#include "lib/readline.h"
#include "lib/strncopy.h"

#define MAXLEN 256
#define MAXLINES 2048

struct linestore {
    struct dynarray *lines;
};

struct linestore *linestore_alloc() {
    struct linestore *s = malloc(sizeof(struct linestore));
    if (s == NULL) {
        return NULL;
    }

    s->lines = dynarray_alloc(0);
    if (s->lines == NULL) {
        free(s);
        return NULL;
    }

    return s;
}

char *linestore_add(struct linestore *s, char *src, size_t len) {
    assert(s != NULL);

    char *dst = malloc((len + 1) * sizeof(char *));
    if (dst == NULL) {
        return NULL;
    }

    strncopy(dst, src, len);
    dst[len] = '\0';

    bool ok = dynarray_append(s->lines, dst);
    if (!ok) {
        free(dst);
        return NULL;
    }

    return dst;
}

void linestore_free(struct linestore *s) {
    assert(s != NULL);

    for (int i = 0; i < dynarray_len(s->lines); i++) {
        free(dynarray_get(s->lines, i));
    }
    dynarray_free(s->lines);
    free(s);
}

struct sorter {
    struct linestore *lines;
    struct heapq *q;
};

struct sorter *sorter_alloc() {
    struct sorter *s = malloc(sizeof(struct sorter));
    if (s == NULL) {
        return NULL;
    }

    s->lines = linestore_alloc();
    if (s->lines == NULL) {
        free(s);
        return NULL;
    }

    s->q = heapq_alloc();
    if (s->q == NULL) {
        free(s->lines);
        free(s);
        return NULL;
    }

    return s;
}

void sorter_free(struct sorter *s) {
    assert(s != NULL);

    free(s->q);
    free(s->lines);
    free(s);
}

bool sorter_push(struct sorter *s, char *src, size_t len) {
    assert(s != NULL);

    char *line = linestore_add(s->lines, src, len);
    if (line == NULL) {
        return false;
    }

    return heapq_push(s->q, line);
}

char *sorter_pop(struct sorter *s) {
    assert(s != NULL);

    if (!heapq_len(s->q)) {
        return NULL;
    }
    return heapq_pop(s->q);
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
        }
        sorter_free(s);
        return EXIT_SUCCESS;
    }

    sorter_free(s);
    fprintf(stderr, "error: too many lines\n");
    return EXIT_FAILURE;
}
