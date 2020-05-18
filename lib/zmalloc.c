#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <unistd.h>

typedef struct block {
    size_t len;
    struct block *next;
} Block;

static int dev_zero;

static bool ensure_dev_zero(void) {
    if (dev_zero) {
        return true;
    }

    errno = 0;
    dev_zero = open("/dev/zero", O_RDWR);
    return !errno;
}

static Block *block_alloc(size_t len) {
    int prot = PROT_READ | PROT_WRITE;

    if (!ensure_dev_zero()) {
        return NULL;
    }

    errno = 0;
    Block *blk = mmap(0, sizeof(Block) + len, prot, MAP_PRIVATE, dev_zero, 0);
    if (errno) {
        return NULL;
    }

    blk->len = len;
    blk->next = NULL;
    return blk;
}

static void block_free(Block *blk) { munmap(blk, sizeof(Block) + blk->len); }

static void *block_mem(Block *blk) { return blk + 1; }

static Block *blocks;

void *zmalloc(size_t len) {
    if (!len) {
        return NULL;
    }

    Block *blk = block_alloc(len);
    if (!blk) {
        return NULL;
    }

    if (blocks) {
        blk->next = blocks;
    }
    blocks = blk;
    return block_mem(blk);
}

void zfree(void *mem) {
    if (!mem) {
        return;
    }

    if (block_mem(blocks) == mem) {
        Block *next = blocks->next;
        block_free(blocks);
        blocks = next;
        return;
    }

    Block *prev, *cur;
    for (prev = blocks, cur = blocks->next;; prev = cur, cur = cur->next) {
        if (block_mem(cur) == mem) {
            prev->next = cur->next;
            block_free(cur);
            return;
        }
    }
}
