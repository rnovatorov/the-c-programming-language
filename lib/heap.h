#ifndef HEAPQ_H
#define HEAPQ_H

#include <stdbool.h>
#include <stdlib.h>

#include "dynarray.h"

void heapify(struct dynarray *d);
bool heap_push(struct dynarray *d, char *s);
char *heap_pop(struct dynarray *d);

#endif
