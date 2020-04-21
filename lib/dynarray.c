#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include <stdio.h>

#define DYNARRAY_GROWTH_FACTOR 2

struct dynarray {
    void **arr;
    size_t len;
    size_t cap;
};

struct dynarray *dynarray_alloc(size_t len) {
    struct dynarray *d = malloc(sizeof(struct dynarray));
    if (d == NULL) {
        return NULL;
    }

    if (len == 0) {
        d->arr = NULL;
        d->len = 0;
        d->cap = 0;
        return d;
    }

    d->arr = malloc(len * sizeof(void *));
    if (d->arr == NULL) {
        free(d);
        return NULL;
    }
    d->len = len;
    d->cap = len;
    return d;
}

void dynarray_free(struct dynarray *d) {
    assert(d != NULL);

    free(d->arr);
    free(d);
}

size_t dynarray_len(struct dynarray *d) {
    assert(d != NULL);

    return d->len;
}

void *dynarray_get(struct dynarray *d, size_t i) {
    assert(d != NULL);
    assert(i < d->len);

    return d->arr[i];
}

void dynarray_set(struct dynarray *d, size_t i, void *v) {
    assert(d != NULL);
    assert(i < d->len);

    d->arr[i] = v;
}

static bool dynarray_resize(struct dynarray *d, size_t cap) {
    if (cap == 0) {
        free(d->arr);
        d->arr = NULL;
        d->cap = 0;
        return true;
    }

    void **arr = realloc(d->arr, cap * sizeof(void *));
    if (arr == NULL) {
        return false;
    }

    d->arr = arr;
    d->cap = cap;
    return true;
}

bool dynarray_append(struct dynarray *d, void *v) {
    assert(d != NULL);

    size_t len = d->len + 1;
    if (len > d->cap) {
        bool ok = dynarray_resize(d, DYNARRAY_GROWTH_FACTOR * len);
        if (!ok) {
            return false;
        }
    }

    d->arr[d->len] = v;
    d->len = len;

    return true;
}

void *dynarray_pop(struct dynarray *d) {
    assert(d != NULL);
    assert(d->len > 0);

    void *v = d->arr[--d->len];

    size_t cap = d->cap / DYNARRAY_GROWTH_FACTOR;
    if (d->len <= cap) {
        dynarray_resize(d, cap);
    }

    return v;
}

void dynarray_swap(struct dynarray *d, size_t i, size_t j) {
    assert(d != NULL);
    assert(i < d->len);
    assert(j < d->len);

    void *tmp = d->arr[i];
    d->arr[i] = d->arr[j];
    d->arr[j] = tmp;
}

int dynarray_index(struct dynarray *d, void *v, bool (*cmp)(void *, void *)) {
    assert(d != NULL);
    assert(cmp != NULL);

    for (int i = 0; i < d->len; i++) {
        if (cmp(d->arr[i], v)) {
            return i;
        }
    }

    return -1;
}
