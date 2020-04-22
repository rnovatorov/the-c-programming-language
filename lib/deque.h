#ifndef DEQUE_H
#define DEQUE_H

#include <stdbool.h>
#include <stdlib.h>

struct deque;

struct deque *deque_alloc(void);
void deque_free(struct deque *d);

size_t deque_len(struct deque *d);
bool deque_append(struct deque *d, void *v);
bool deque_appendleft(struct deque *d, void *v);
void *deque_pop(struct deque *d);
void *deque_popleft(struct deque *d);

#endif
