#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/dynarray.h"

bool cmp(void *a, void *b) { return !strcmp(a, b); }

int main() {
    struct dynarray *d = dynarray_alloc(0);
    assert(dynarray_len(d) == 0);
    assert(dynarray_index(d, "wut", &cmp) == -1);

    assert(dynarray_append(d, "foo"));
    assert(dynarray_len(d) == 1);
    assert(dynarray_index(d, "foo", &cmp) == 0);
    assert(!strcmp(dynarray_get(d, 0), "foo"));

    assert(dynarray_append(d, "bar"));
    assert(dynarray_len(d) == 2);
    assert(dynarray_index(d, "foo", &cmp) == 0);
    assert(dynarray_index(d, "bar", &cmp) == 1);
    assert(!strcmp(dynarray_get(d, 0), "foo"));
    assert(!strcmp(dynarray_get(d, 1), "bar"));

    dynarray_swap(d, 0, 1);
    assert(dynarray_len(d) == 2);
    assert(dynarray_index(d, "bar", &cmp) == 0);
    assert(dynarray_index(d, "foo", &cmp) == 1);
    assert(!strcmp(dynarray_get(d, 0), "bar"));
    assert(!strcmp(dynarray_get(d, 1), "foo"));

    assert(!strcmp(dynarray_pop(d), "foo"));
    assert(dynarray_len(d) == 1);
    assert(!strcmp(dynarray_get(d, 0), "bar"));

    assert(!strcmp(dynarray_pop(d), "bar"));
    assert(dynarray_len(d) == 0);

    dynarray_free(d);

    return EXIT_SUCCESS;
}
