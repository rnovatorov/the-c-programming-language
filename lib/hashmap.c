#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// TODO: Resize.
// #define HASHMAP_GROW_FACTOR 2
// #define HASHMAP_LOAD_THRESHOLD 4

struct entry {
    unsigned long hash;
    void *value;
    struct entry *next;
};

static struct entry *entry_alloc(unsigned long hash, void *value) {
    struct entry *e = malloc(sizeof(struct entry));
    if (e == NULL) {
        return NULL;
    }
    e->hash = hash;
    e->value = value;
    e->next = NULL;
    return e;
}

static void entry_free(struct entry *e) {
    assert(e != NULL);

    free(e);
}

struct hashmap {
    struct entry **buckets;
    size_t num_buckets;
    size_t num_entries;
    unsigned long (*hashfunc)(char *);
};

struct hashmap_params {
    size_t num_buckets;
    unsigned long (*hashfunc)(char *);
};

struct hashmap *hashmap_alloc(struct hashmap_params params) {
    assert(params.num_buckets > 0);
    assert(params.hashfunc != NULL);

    struct hashmap *m = malloc(sizeof(struct hashmap));
    if (m == NULL) {
        return NULL;
    }

    m->num_buckets = params.num_buckets;
    m->buckets = calloc(m->num_buckets, sizeof(struct entry *));
    if (m->buckets == NULL) {
        free(m);
        return NULL;
    }

    m->num_entries = 0;
    m->hashfunc = params.hashfunc;

    return m;
}

static void bucket_free(struct hashmap *m, size_t b) {
    struct entry *e = m->buckets[b];
    if (e == NULL) {
        return;
    }
    for (struct entry *next; e != NULL; e = next) {
        next = e->next;
        entry_free(e);
    }
}

void hashmap_free(struct hashmap *m) {
    assert(m != NULL);

    for (size_t b = 0; b < m->num_buckets; b++) {
        bucket_free(m, b);
    }
    free(m->buckets);
    free(m);
}

size_t hashmap_len(struct hashmap *m) { return m->num_entries; }

// TODO: Resize on high load.
// static float hashmap_get_load(struct hashmap *m) {
//    assert(m != NULL);
//    assert(m->num_buckets != 0);
//
//    return (float)m->num_entries / m->num_buckets;
//}

enum bucket_add_status { BUCKET_ADD_ERROR, BUCKET_ADD_RESET, BUCKET_ADD_NEW };

static enum bucket_add_status bucket_add(struct hashmap *m, size_t b,
                                         unsigned long hash, void *value) {
    struct entry *e = m->buckets[b];
    if (e == NULL) {
        e = entry_alloc(hash, value);
        if (e == NULL) {
            return BUCKET_ADD_ERROR;
        }
        m->buckets[b] = e;
        return BUCKET_ADD_NEW;
    }

    for (;; e = e->next) {
        if (e->hash == hash) {
            e->value = value;
            return BUCKET_ADD_RESET;
        }
        if (!e->next) {
            break;
        }
    }

    e->next = entry_alloc(hash, value);
    if (e->next == NULL) {
        return BUCKET_ADD_ERROR;
    }
    return BUCKET_ADD_NEW;
}

bool hashmap_set(struct hashmap *m, char *key, void *value) {
    assert(m != NULL);

    unsigned long hash = m->hashfunc(key);
    size_t b = hash % m->num_buckets;

    enum bucket_add_status s = bucket_add(m, b, hash, value);
    if (s == BUCKET_ADD_ERROR) {
        return false;
    }
    if (s == BUCKET_ADD_NEW) {
        m->num_entries++;
    }
    return true;
}

struct hashmap_lookup {
    void *value;
    bool ok;
};

static struct hashmap_lookup bucket_get(struct hashmap *m, size_t b,
                                        unsigned long hash) {
    for (struct entry *e = m->buckets[b]; e != NULL; e = e->next) {
        if (e->hash == hash) {
            struct hashmap_lookup lkp = {.value = e->value, .ok = true};
            return lkp;
        }
    }
    struct hashmap_lookup lkp = {.value = NULL, .ok = false};
    return lkp;
}

struct hashmap_lookup hashmap_get(struct hashmap *m, char *key) {
    assert(m != NULL);

    unsigned long hash = m->hashfunc(key);
    size_t b = hash % m->num_buckets;

    return bucket_get(m, b, hash);
}

static bool bucket_delete(struct hashmap *m, size_t b, unsigned long hash) {
    struct entry *e = m->buckets[b];
    if (e == NULL) {
        return false;
    }

    if (e->hash == hash) {
        struct entry *next = e->next;
        m->buckets[b] = next;
        entry_free(e);
        return true;
    }

    struct entry *prev, *cur;
    for (prev = e, cur = prev->next; cur != NULL; prev = cur, cur = cur->next) {
        if (cur->hash == hash) {
            prev->next = cur->next;
            entry_free(cur);
            return true;
        }
    }

    return false;
}

bool hashmap_delete(struct hashmap *m, char *key) {
    assert(m != NULL);

    unsigned long hash = m->hashfunc(key);
    size_t b = hash % m->num_buckets;

    bool deleted = bucket_delete(m, b, hash);
    if (deleted) {
        m->num_entries--;
    }
    return deleted;
}
