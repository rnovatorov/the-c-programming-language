#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "lib/heapq.h"

int main() {
    struct heapq *q = heapq_alloc();
    assert(heapq_len(q) == 0);

    heapq_push(q, "c");
    assert(heapq_len(q) == 1);

    heapq_push(q, "a");
    assert(heapq_len(q) == 2);

    heapq_push(q, "b");
    assert(heapq_len(q) == 3);

    assert(!strcmp(heapq_pop(q), "a"));
    assert(heapq_len(q) == 2);

    assert(!strcmp(heapq_pop(q), "b"));
    assert(heapq_len(q) == 1);

    assert(!strcmp(heapq_pop(q), "c"));
    assert(heapq_len(q) == 0);

    heapq_free(q);

    return EXIT_SUCCESS;
}
