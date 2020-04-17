#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct item {
    struct item *prev;
    struct item *next;
    char *s;
};

static struct item *item_alloc(char *s) {
    struct item *i = malloc(sizeof(struct item));
    if (i == NULL) {
        return false;
    }

    i->next = NULL;
    i->prev = NULL;
    i->s = s;
    return i;
}

static void item_free(struct item *i) {
    assert(i != NULL);

    free(i);
}

struct deque {
    struct item *first;
    struct item *last;
    size_t len;
};

struct deque *deque_alloc(void) {
    struct deque *d = malloc(sizeof(struct deque));
    if (d == NULL) {
        return NULL;
    }

    d->first = NULL;
    d->last = NULL;
    d->len = 0;
    return d;
}

void deque_free(struct deque *d) {
    assert(d != NULL);

    struct item *i = d->first;
    while (i) {
        struct item *next = i->next;
        item_free(i);
        i = next;
    }
    free(d);
}

size_t deque_len(struct deque *d) {
    assert(d != NULL);

    return d->len;
}

bool deque_append(struct deque *d, char *s) {
    assert(d != NULL);

    struct item *i = item_alloc(s);
    if (i == NULL) {
        return false;
    }
    d->len++;

    if (d->last == NULL) {
        assert(d->first == NULL);
        d->first = i;
        d->last = i;
        return true;
    }

    assert(d->last->next == NULL);
    d->last->next = i;
    i->prev = d->last;
    d->last = i;
    return true;
}

bool deque_appendleft(struct deque *d, char *s) {
    assert(d != NULL);

    struct item *i = item_alloc(s);
    if (i == NULL) {
        return false;
    }
    d->len++;

    if (d->first == NULL) {
        assert(d->last == NULL);
        d->first = i;
        d->last = i;
        return true;
    }

    assert(d->first->prev == NULL);
    d->first->prev = i;
    i->next = d->first;
    d->first = i;
    return true;
}

char *deque_pop(struct deque *d) {
    assert(d != NULL);
    assert(d->len != 0);

    struct item *i = d->last;
    assert(i != NULL);

    if (i->prev == NULL) {
        assert(d->len == 1);
        assert(d->first == i);
        assert(d->last == i);

        d->first = NULL;
        d->last = NULL;
    } else {
        assert(d->len > 1);
        assert(i->prev->next == i);
        i->prev->next = NULL;
        d->last = i->prev;
    }

    char *s = i->s;
    item_free(i);
    d->len--;
    return s;
}

char *deque_popleft(struct deque *d) {
    assert(d != NULL);
    assert(d->len != 0);

    struct item *i = d->first;
    assert(i != NULL);

    if (i->next == NULL) {
        assert(d->len == 1);
        assert(d->first == i);
        assert(d->last == i);

        d->first = NULL;
        d->last = NULL;
    } else {
        assert(d->len > 1);
        assert(i->next->prev == i);
        i->next->prev = NULL;
        d->first = i->next;
    }

    char *s = i->s;
    item_free(i);
    d->len--;
    return s;
}
