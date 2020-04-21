#include <assert.h>
#include <string.h>

#include "../lib/djb2.h"
#include "../lib/hashmap.h"

int main() {
    struct hashmap_lookup lkp;

    // alloc
    struct hashmap *m = hashmap_alloc(&djb2);
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

    // free
    hashmap_free(m);
}
