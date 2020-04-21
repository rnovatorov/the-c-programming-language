#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <stdbool.h>
#include <stdlib.h>

struct dynarray;

struct dynarray *dynarray_alloc(size_t len);
void dynarray_free(struct dynarray *d);

size_t dynarray_len(struct dynarray *d);
void *dynarray_get(struct dynarray *d, size_t i);
void dynarray_set(struct dynarray *d, size_t i, void *v);
bool dynarray_append(struct dynarray *d, void *v);
void *dynarray_pop(struct dynarray *d);
void dynarray_swap(struct dynarray *d, size_t i, size_t j);
int dynarray_index(struct dynarray *d, void *v, bool (*cmp)(void *, void *));

#endif
