#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynarray.h"

struct heapq {
    struct dynarray *d;
};

struct heapq *heapq_alloc(void) {
    struct heapq *q = malloc(sizeof(struct heapq));
    if (q == NULL) {
        return NULL;
    }

    q->d = dynarray_alloc(0);
    if (q->d == NULL) {
        free(q);
        return NULL;
    }

    return q;
}

void heapq_free(struct heapq *q) {
    assert(q != NULL);

    dynarray_free(q->d);
    free(q);
}

size_t heapq_len(struct heapq *q) {
    assert(q != NULL);

    return dynarray_len(q->d);
}

static int heapq_cmp(struct heapq *q, size_t i, size_t j) {
    return strcmp(dynarray_get(q->d, i), dynarray_get(q->d, j));
}

static size_t parent(size_t i) { return (i - 1) / 2; }
static size_t left_child(size_t i) { return (2 * i) + 1; }
static size_t right_child(size_t i) { return (2 * i) + 2; }

static void heapq_heapify_up(struct heapq *q) {
    size_t p;
    for (size_t c = heapq_len(q) - 1; c; c = p) {
        p = parent(c);
        if (heapq_cmp(q, p, c) < 0) {
            return;
        }
        dynarray_swap(q->d, c, p);
    }
}

bool heapq_push(struct heapq *q, char *s) {
    assert(q != NULL);

    bool ok = dynarray_append(q->d, s);
    if (!ok) {
        return false;
    }
    heapq_heapify_up(q);
    return true;
}

void static heapq_heapify_down(struct heapq *q) {
    size_t c;
    for (size_t p = 0;; p = c) {
        size_t l = left_child(p);
        size_t r = right_child(p);

        if (r < heapq_len(q)) {
            c = heapq_cmp(q, l, r) < 0 ? l : r;
        } else if (l < heapq_len(q)) {
            c = l;
        } else {
            return;
        }

        if (heapq_cmp(q, p, c) < 0) {
            return;
        }
        dynarray_swap(q->d, c, p);
    }
}

char *heapq_pop(struct heapq *q) {
    assert(q != NULL);

    dynarray_swap(q->d, 0, heapq_len(q) - 1);
    char *s = dynarray_pop(q->d);

    heapq_heapify_down(q);
    return s;
}
