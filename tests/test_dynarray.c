#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../lib/dynarray.h"

int main() {
    struct dynarray *d = dynarray_alloc(0);
    assert(dynarray_len(d) == 0);

    char *const foo = "foo";
    assert(dynarray_append(d, foo));
    assert(dynarray_len(d) == 1);
    assert(dynarray_get(d, 0) == foo);

    char *const bar = "bar";
    assert(dynarray_append(d, bar));
    assert(dynarray_len(d) == 2);
    assert(dynarray_get(d, 0) == foo);
    assert(dynarray_get(d, 1) == bar);

    dynarray_set(d, 0, bar);
    dynarray_set(d, 1, foo);
    assert(dynarray_len(d) == 2);
    assert(dynarray_get(d, 0) == bar);
    assert(dynarray_get(d, 1) == foo);

    assert(dynarray_pop(d) == foo);
    assert(dynarray_len(d) == 1);
    assert(dynarray_get(d, 0) == bar);

    assert(dynarray_pop(d) == bar);
    assert(dynarray_len(d) == 0);

    dynarray_free(d);

    return EXIT_SUCCESS;
}
