#include <assert.h>
#include <string.h>

#include "../lib/djb2.h"
#include "../lib/hashmap.h"

int main() {
    struct hashmap_lookup lkp;

    // alloc
    struct hashmap_params params = {
        .num_buckets = 8,
        .load_threshold = 2,
        .grow_factor = 2,
        .hashfunc = &djb2,
    };
    struct hashmap *m = hashmap_alloc(params);
    assert(hashmap_len(m) == 0);

    // set and get
    assert(hashmap_set(m, "foo", "bar"));
    assert(hashmap_len(m) == 1);

    lkp = hashmap_get(m, "foo");
    assert(lkp.ok);
    assert(!strcmp(lkp.value, "bar"));

    // get non existent
    lkp = hashmap_get(m, "ham");
    assert(!lkp.ok);
    assert(lkp.value == NULL);

    // set another
    assert(hashmap_set(m, "ham", "cheese"));
    assert(hashmap_len(m) == 2);

    lkp = hashmap_get(m, "foo");
    assert(lkp.ok);
    assert(!strcmp(lkp.value, "bar"));

    lkp = hashmap_get(m, "ham");
    assert(lkp.ok);
    assert(!strcmp(lkp.value, "cheese"));

    // reset
    hashmap_set(m, "foo", "goo");
    assert(hashmap_len(m) == 2);

    lkp = hashmap_get(m, "foo");
    assert(lkp.ok);
    assert(!strcmp(lkp.value, "goo"));

    lkp = hashmap_get(m, "ham");
    assert(lkp.ok);
    assert(!strcmp(lkp.value, "cheese"));

    // delete non existent
    assert(!hashmap_delete(m, "wut"));
    assert(hashmap_len(m) == 2);

    lkp = hashmap_get(m, "foo");
    assert(lkp.ok);
    assert(!strcmp(lkp.value, "goo"));

    lkp = hashmap_get(m, "ham");
    assert(lkp.ok);
    assert(!strcmp(lkp.value, "cheese"));

    // delete existent
    assert(hashmap_delete(m, "foo"));
    assert(hashmap_len(m) == 1);

    lkp = hashmap_get(m, "foo");
    assert(!lkp.ok);
    assert(lkp.value == NULL);

    lkp = hashmap_get(m, "ham");
    assert(lkp.ok);
    assert(!strcmp(lkp.value, "cheese"));

    // delete all
    assert(hashmap_delete(m, "ham"));
    assert(hashmap_len(m) == 0);

    lkp = hashmap_get(m, "foo");
    assert(!lkp.ok);
    assert(lkp.value == NULL);

    lkp = hashmap_get(m, "ham");
    assert(!lkp.ok);
    assert(lkp.value == NULL);

    // grow
    assert(hashmap_set(m, "answer", "42"));

    char buf[2] = {0};
    char i = 1;
    for (; i < params.num_buckets * params.load_threshold; i++) {
        buf[0] = 'A' + i;
        hashmap_set(m, buf, "grow");

        assert(hashmap_len(m) == i + 1);
        assert(hashmap_num_buckets(m) == params.num_buckets);

        lkp = hashmap_get(m, "answer");
        assert(lkp.ok);
        assert(!strcmp(lkp.value, "42"));
    }

    buf[0] = 'A' + i;
    hashmap_set(m, buf, "grow");

    assert(hashmap_len(m) == i + 1);
    assert(hashmap_num_buckets(m) == params.grow_factor * params.num_buckets);

    lkp = hashmap_get(m, "answer");
    assert(lkp.ok);
    assert(!strcmp(lkp.value, "42"));

    // collision
    char *k1 = "1r";
    char *k2 = "30";
    assert(djb2(k1) == djb2(k2));

    hashmap_set(m, k1, "foo");
    hashmap_set(m, k2, "bar");

    lkp = hashmap_get(m, k1);
    assert(lkp.ok);
    assert(!strcmp(lkp.value, "foo"));

    lkp = hashmap_get(m, k2);
    assert(lkp.ok);
    assert(!strcmp(lkp.value, "bar"));

    // free
    hashmap_free(m);
}
