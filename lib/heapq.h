#ifndef HEAPQ_H
#define HEAPQ_H

#include <stdbool.h>
#include <stdlib.h>

struct heapq;

struct heapq *heapq_alloc(void);
void heapq_free(struct heapq *q);

size_t heapq_len(struct heapq *q);
bool heapq_push(struct heapq *q, char *s);
char *heapq_pop(struct heapq *q);

#endif
