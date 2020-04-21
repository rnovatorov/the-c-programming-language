#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include <stddef.h>

struct hashmap;

struct hashmap *hashmap_alloc(unsigned long (*hashfunc)(char *));
void hashmap_free(struct hashmap *m);

size_t hashmap_len(struct hashmap *m);

struct hashmap_lookup {
    void *value;
    bool ok;
};

bool hashmap_set(struct hashmap *m, char *key, void *value);
struct hashmap_lookup hashmap_get(struct hashmap *m, char *key);
bool hashmap_delete(struct hashmap *m, char *key);

#endif
