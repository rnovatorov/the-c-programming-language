#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include <stddef.h>

struct hashmap;

struct hashmap_params {
    size_t num_buckets;
    size_t load_threshold;
    size_t grow_factor;
    unsigned long (*hashfunc)(char *);
};
struct hashmap *hashmap_alloc(struct hashmap_params params);

void hashmap_free(struct hashmap *m);

size_t hashmap_len(struct hashmap *m);
size_t hashmap_num_buckets(struct hashmap *m);

struct hashmap_lookup {
    void *value;
    bool ok;
};

bool hashmap_set(struct hashmap *m, char *key, void *value);
struct hashmap_lookup hashmap_get(struct hashmap *m, char *key);
bool hashmap_delete(struct hashmap *m, char *key);

#endif
