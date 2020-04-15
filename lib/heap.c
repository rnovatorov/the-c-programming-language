#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynarray.h"

static int compare(struct dynarray *d, size_t i, size_t j) {
    return strcmp(dynarray_get(d, i), dynarray_get(d, j));
}

static size_t parent(size_t i) { return (i - 1) / 2; }
static size_t left_child(size_t i) { return (2 * i) + 1; }
static size_t right_child(size_t i) { return (2 * i) + 2; }

static void sift_up(struct dynarray *d) {
    size_t p;
    for (size_t c = dynarray_len(d) - 1; c; c = p) {
        p = parent(c);
        if (compare(d, p, c) < 0) {
            return;
        }
        dynarray_swap(d, c, p);
    }
}

bool heap_push(struct dynarray *d, char *s) {
    assert(d != NULL);

    bool ok = dynarray_append(d, s);
    if (!ok) {
        return false;
    }
    sift_up(d);
    return true;
}

void static sift_down(struct dynarray *d, size_t p) {
    size_t c;
    for (;;) {
        size_t l = left_child(p);
        size_t r = right_child(p);

        if (r < dynarray_len(d)) {
            c = compare(d, l, r) < 0 ? l : r;
        } else if (l < dynarray_len(d)) {
            c = l;
        } else {
            return;
        }

        if (compare(d, p, c) < 0) {
            return;
        }
        dynarray_swap(d, c, p);
        p = c;
    }
}

char *heap_pop(struct dynarray *d) {
    assert(d != NULL);

    dynarray_swap(d, 0, dynarray_len(d) - 1);
    char *s = dynarray_pop(d);

    sift_down(d, 0);
    return s;
}

void heapify(struct dynarray *d) {
    assert(d != NULL);

    if (dynarray_len(d) <= 1) {
        return;
    }

    for (size_t i = parent(dynarray_len(d) - 1);;) {
        sift_down(d, i);
        if (!i--) {
            return;
        }
    }
}
