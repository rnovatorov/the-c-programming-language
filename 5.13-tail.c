#include <assert.h>
#include <errno.h>
#include <stdlib.h>

#include "lib/deque.h"
#include "lib/readline.h"
#include "lib/strncopy.h"

#define MAXLEN 8

struct tailer {
    struct deque *d;
    size_t numlines;
};

struct tailer *tailer_alloc(size_t numlines) {
    struct tailer *t = malloc(sizeof(struct tailer));
    if (t == NULL) {
        return NULL;
    }

    t->d = deque_alloc();
    if (t->d == NULL) {
        free(t);
        return NULL;
    }

    t->numlines = numlines;
    return t;
}

void tailer_free(struct tailer *t) {
    assert(t != NULL);

    while (deque_len(t->d)) {
        free(deque_pop(t->d));
    }
    deque_free(t->d);
    free(t);
}

bool tailer_push(struct tailer *t, char *buf, size_t len) {
    assert(t != NULL);

    char *line = malloc((len + 1) * sizeof(char));
    if (line == NULL) {
        return false;
    }

    strncopy(line, buf, len);
    line[len] = '\0';

    bool ok = deque_append(t->d, line);
    if (!ok) {
        free(line);
        return false;
    }

    if (deque_len(t->d) > t->numlines) {
        free(deque_popleft(t->d));
    }
    return true;
}

char *tailer_pop(struct tailer *t) {
    assert(t != NULL);

    if (!deque_len(t->d)) {
        return NULL;
    }
    return deque_popleft(t->d);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s NUMLINES\n", argv[0]);
        return EXIT_FAILURE;
    }
    char *end = NULL;
    errno = 0;
    long numlines = strtol(argv[1], &end, 10);
    if (*end != '\0') {
        fprintf(stderr, "error: numlines is not a number\n");
        return EXIT_FAILURE;
    }
    if (errno == ERANGE) {
        fprintf(stderr, "error: numlines is out of range\n");
        return EXIT_FAILURE;
    }
    assert(!errno);

    struct tailer *t = tailer_alloc(numlines);
    if (t == NULL) {
        fprintf(stderr, "error: failed to allocate tailer\n");
        return EXIT_FAILURE;
    }

    char buf[MAXLEN];
    int nread;
    while ((nread = readline(buf, MAXLEN, stdin))) {
        if (nread < 0) {
            tailer_free(t);
            fprintf(stderr, "error: line too long\n");
            return EXIT_FAILURE;
        }
        if (nread > 0) {
            bool ok = tailer_push(t, buf, nread);
            if (!ok) {
                tailer_free(t);
                fprintf(stderr, "error: failed to push to tailer\n");
                return EXIT_FAILURE;
            }
        }
    }

    char *line;
    while ((line = tailer_pop(t))) {
        printf("%s", line);
        free(line);
    }
    tailer_free(t);
    return EXIT_SUCCESS;
}
