#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/dynarray.h"
#include "../lib/heap.h"

int main() {
    struct dynarray *d = dynarray_alloc(0);

    heap_push(d, "c");
    heap_push(d, "a");
    heap_push(d, "b");
    assert(dynarray_len(d) == 3);

    assert(!strcmp(heap_pop(d), "a"));
    assert(!strcmp(heap_pop(d), "b"));
    assert(!strcmp(heap_pop(d), "c"));
    assert(!dynarray_len(d));

    dynarray_append(d, "c");
    dynarray_append(d, "d");
    dynarray_append(d, "b");
    dynarray_append(d, "a");
    assert(dynarray_len(d) == 4);

    heapify(d);

    assert(!strcmp(heap_pop(d), "a"));
    assert(!strcmp(heap_pop(d), "b"));
    assert(!strcmp(heap_pop(d), "c"));
    assert(!strcmp(heap_pop(d), "d"));
    assert(!dynarray_len(d));

    dynarray_free(d);

    return EXIT_SUCCESS;
}
