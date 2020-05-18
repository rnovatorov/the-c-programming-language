#ifndef ZMALLOC_H
#define ZMALLOC_H

#include <stddef.h>

void *zmalloc(size_t len);
void zfree(void *mem);

#endif
