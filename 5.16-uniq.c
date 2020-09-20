#include <assert.h>
#include <stdlib.h>

#include "lib/djb2.h"
#include "lib/dynarray.h"
#include "lib/hashmap.h"
#include "lib/readline.h"
#include "lib/strncopy.h"

#define MAXLEN 8

struct linestore {
    struct dynarray *lines;
};

struct linestore *linestore_alloc(void) {
    struct linestore *s = malloc(sizeof(struct linestore));
    if (s == NULL) {
        return NULL;
    }

    s->lines = dynarray_alloc(0);
    if (s->lines == NULL) {
        free(s);
        return NULL;
    }
    return s;
}

void linestore_free(struct linestore *s) {
    assert(s != NULL);

    for (size_t i = 0; i < dynarray_len(s->lines); i++) {
        free(dynarray_get(s->lines, i));
    }
    dynarray_free(s->lines);
    free(s);
}

int main() {
    struct linestore *s = linestore_alloc();

    struct hashmap_params params = {
        .num_buckets = 8,
        .hashfunc = &djb2,
    };
    struct hashmap *m = hashmap_alloc(params);

    char buf[MAXLEN];
    int nread;
    while ((nread = readline(buf, MAXLEN, stdin))) {
        if (nread < 0) {
            fprintf(stderr, "error: line too long\n");
            return EXIT_FAILURE;
        }
        if (nread > 0) {
            char *line = malloc((nread + 1) * sizeof(char));
            if (line == NULL) {
                linestore_free(s);
                hashmap_free(m);
                fprintf(stderr, "error: failed to alloc line\n");
                return EXIT_FAILURE;
            }
            strncopy(line, buf, nread);
            line[nread] = '\0';

            struct hashmap_lookup lkp = hashmap_get(m, line);
            size_t v;
            if (lkp.ok) {
                v = (size_t)lkp.value;
            } else {
                v = 0;
                bool ok = dynarray_append(s->lines, line);
                if (!ok) {
                    linestore_free(s);
                    hashmap_free(m);
                    fprintf(stderr, "error: failed to store line\n");
                    return EXIT_FAILURE;
                }
            }

            bool ok = hashmap_set(m, line, (void *)++v);
            if (lkp.ok) {
                free(line);
            }
            if (!ok) {
                linestore_free(s);
                hashmap_free(m);
                fprintf(stderr, "error: failed to set line counter\n");
                return EXIT_FAILURE;
            }
        }
    }

    for (int i = 0; i < dynarray_len(s->lines); i++) {
        char *line = dynarray_get(s->lines, i);
        struct hashmap_lookup lkp = hashmap_get(m, line);
        if (!lkp.ok) {
            linestore_free(s);
            hashmap_free(m);
            fprintf(stderr, "panic: unexpected lookup miss\n");
            return EXIT_FAILURE;
        }
        printf("%2lu\t%s", (size_t)lkp.value, line);
    }

    linestore_free(s);
    hashmap_free(m);
    return EXIT_SUCCESS;
}
