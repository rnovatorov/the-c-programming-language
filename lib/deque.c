#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct node {
    struct node *prev;
    struct node *next;
    void *v;
};

static struct node *node_alloc(void *v) {
    struct node *n = malloc(sizeof(struct node));
    if (n == NULL) {
        return false;
    }

    n->next = NULL;
    n->prev = NULL;
    n->v = v;
    return n;
}

static void node_free(struct node *n) { free(n); }

struct deque {
    struct node *first;
    struct node *last;
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

    struct node *n = d->first;
    while (n) {
        struct node *next = n->next;
        node_free(n);
        n = next;
    }
    free(d);
}

size_t deque_len(struct deque *d) {
    assert(d != NULL);

    return d->len;
}

bool deque_append(struct deque *d, void *v) {
    assert(d != NULL);

    struct node *n = node_alloc(v);
    if (n == NULL) {
        return false;
    }
    d->len++;

    if (d->last == NULL) {
        assert(d->first == NULL);
        d->first = n;
        d->last = n;
        return true;
    }

    assert(d->last->next == NULL);
    d->last->next = n;
    n->prev = d->last;
    d->last = n;
    return true;
}

bool deque_appendleft(struct deque *d, void *v) {
    assert(d != NULL);

    struct node *n = node_alloc(v);
    if (n == NULL) {
        return false;
    }
    d->len++;

    if (d->first == NULL) {
        assert(d->last == NULL);
        d->first = n;
        d->last = n;
        return true;
    }

    assert(d->first->prev == NULL);
    d->first->prev = n;
    n->next = d->first;
    d->first = n;
    return true;
}

char *deque_pop(struct deque *d) {
    assert(d != NULL);
    assert(d->len != 0);

    struct node *n = d->last;
    assert(n != NULL);

    if (n->prev == NULL) {
        assert(d->len == 1);
        d->first = NULL;
        d->last = NULL;
    } else {
        assert(d->len > 1);
        n->prev->next = NULL;
        d->last = n->prev;
    }

    void *v = n->v;
    node_free(n);
    d->len--;
    return v;
}

char *deque_popleft(struct deque *d) {
    assert(d != NULL);
    assert(d->len != 0);

    struct node *n = d->first;
    assert(n != NULL);

    if (n->next == NULL) {
        assert(d->len == 1);
        d->first = NULL;
        d->last = NULL;
    } else {
        assert(d->len > 1);
        n->next->prev = NULL;
        d->first = n->next;
    }

    void *v = n->v;
    node_free(n);
    d->len--;
    return v;
}
