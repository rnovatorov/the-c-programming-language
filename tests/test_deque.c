#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/deque.h"

int main() {
    struct deque *d = deque_alloc();
    assert(deque_len(d) == 0);

    assert(deque_append(d, "a"));
    assert(deque_append(d, "b"));
    assert(deque_append(d, "c"));
    assert(deque_len(d) == 3);

    assert(!strcmp(deque_popleft(d), "a"));
    assert(!strcmp(deque_popleft(d), "b"));
    assert(!strcmp(deque_popleft(d), "c"));
    assert(deque_len(d) == 0);

    assert(deque_appendleft(d, "a"));
    assert(deque_appendleft(d, "b"));
    assert(deque_appendleft(d, "c"));
    assert(deque_len(d) == 3);

    assert(!strcmp(deque_pop(d), "a"));
    assert(!strcmp(deque_pop(d), "b"));
    assert(!strcmp(deque_pop(d), "c"));
    assert(deque_len(d) == 0);

    deque_free(d);

    return EXIT_SUCCESS;
}
