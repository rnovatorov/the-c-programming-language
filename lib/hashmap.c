#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <stdio.h>

#define HASHMAP_INIT_BUCKETS 8

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

struct bucket {
    struct entry *first;
};

static struct bucket *bucket_alloc(void) {
    struct bucket *b = malloc(sizeof(struct bucket));
    if (b == NULL) {
        return NULL;
    }
    b->first = NULL;
    return b;
}

static void bucket_free(struct bucket *b) {
    assert(b != NULL);

    for (struct entry *next, *e = b->first; e; e = next) {
        next = e->next;
        entry_free(e);
    }
    free(b);
}

enum bucket_add_status { BUCKET_ADD_ERROR, BUCKET_ADD_RESET, BUCKET_ADD_NEW };

static enum bucket_add_status bucket_add(struct bucket *b, unsigned long hash,
                                         void *value) {
    assert(b != NULL);

    if (b->first == NULL) {
        b->first = entry_alloc(hash, value);
        if (b->first == NULL) {
            return BUCKET_ADD_ERROR;
        }
        return BUCKET_ADD_NEW;
    }

    struct entry *e;
    for (e = b->first;; e = e->next) {
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

struct hashmap_lookup {
    void *value;
    bool ok;
};

static struct hashmap_lookup bucket_get(struct bucket *b, unsigned long hash) {
    assert(b != NULL);

    struct hashmap_lookup lkp = {.value = NULL, .ok = false};

    for (struct entry *e = b->first; e; e = e->next) {
        if (e->hash == hash) {
            lkp.value = e->value;
            lkp.ok = true;
            return lkp;
        }
    }

    return lkp;
}

static bool bucket_delete(struct bucket *b, unsigned long hash) {
    assert(b != NULL);

    if (b->first == NULL) {
        return false;
    }

    if (b->first->hash == hash) {
        struct entry *next = b->first->next;
        entry_free(b->first);
        b->first = next;
        return true;
    }

    struct entry *prev, *cur;
    for (prev = b->first, cur = prev->next; cur; prev = cur, cur = cur->next) {
        if (cur->hash == hash) {
            prev->next = cur->next;
            entry_free(cur);
            return true;
        }
    }

    return false;
}

struct hashmap {
    struct bucket **buckets;
    size_t num_buckets;
    size_t num_entries;
    unsigned long (*hashfunc)(char *);
};

struct hashmap *hashmap_alloc(unsigned long (*hashfunc)(char *)) {
    assert(hashfunc != NULL);

    struct hashmap *m = malloc(sizeof(struct hashmap));
    if (m == NULL) {
        return NULL;
    }

    m->num_buckets = HASHMAP_INIT_BUCKETS;
    m->buckets = calloc(m->num_buckets, sizeof(struct bucket *));
    if (m->buckets == NULL) {
        free(m);
        return NULL;
    }

    m->num_entries = 0;
    m->hashfunc = hashfunc;

    return m;
}

void hashmap_free(struct hashmap *m) {
    assert(m != NULL);

    for (size_t i = 0; i < m->num_buckets; i++) {
        struct bucket *b = m->buckets[i];
        if (b != NULL) {
            bucket_free(b);
        }
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

bool hashmap_set(struct hashmap *m, char *key, void *value) {
    assert(m != NULL);

    unsigned long hash = m->hashfunc(key);
    size_t index = hash % m->num_buckets;

    struct bucket *b = m->buckets[index];
    if (b == NULL) {
        b = bucket_alloc();
        if (b == NULL) {
            return false;
        }
        m->buckets[index] = b;
    }

    enum bucket_add_status s = bucket_add(b, hash, value);
    if (s == BUCKET_ADD_ERROR) {
        return false;
    }
    if (s == BUCKET_ADD_NEW) {
        m->num_entries++;
    }
    return true;
}

struct hashmap_lookup hashmap_get(struct hashmap *m, char *key) {
    assert(m != NULL);

    unsigned long hash = m->hashfunc(key);
    size_t index = hash % m->num_buckets;
    struct bucket *b = m->buckets[index];
    if (b == NULL) {
        struct hashmap_lookup lkp = {.value = NULL, .ok = false};
        return lkp;
    }

    return bucket_get(b, hash);
}

bool hashmap_delete(struct hashmap *m, char *key) {
    assert(m != NULL);

    unsigned long hash = m->hashfunc(key);
    size_t index = hash % m->num_buckets;

    struct bucket *b = m->buckets[index];
    if (b == NULL) {
        return false;
    }

    bool deleted = bucket_delete(b, hash);
    if (deleted) {
        m->num_entries--;
    }
    return deleted;
}
