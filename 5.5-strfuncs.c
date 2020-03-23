#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

int ncmp(char *a, char *b, int n) {
    int delta = *b - *a;
    int i = 0;
    while (i < n && !delta && *a) {
        delta = *b - *a;
        a++, b++, i++;
    }
    return delta;
}

void test_ncmp() {
    assert(ncmp("ba", "aa", 2) < 0);
    assert(ncmp("ba", "ab", 2) < 0);
    assert(ncmp("bb", "ba", 2) < 0);

    assert(ncmp("ab", "ab", 2) == 0);
    assert(ncmp("ba", "ba", 2) == 0);
    assert(ncmp("aaa", "aab", 2) == 0);

    assert(ncmp("aa", "ba", 2) > 0);
    assert(ncmp("ab", "ba", 2) > 0);
    assert(ncmp("ba", "bb", 2) > 0);
}

int ncopy(char *dst, char *src, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (!(*dst++ = *src++)) {
            break;
        }
    }
    return i;
}

void test_ncopy_n_lt() {
    char to[] = "12345678";
    char from[] = "abcd";
    ncopy(to, from, 2);
    assert(!(ncmp(to, "ab345678", 8)));
}

void test_ncopy_n_eq() {
    char to[] = "12345678";
    char from[] = "abcd";
    ncopy(to, from, 4);
    assert(!(ncmp(to, "abcd5678", 8)));
}

void test_ncopy_n_gt() {
    char to[] = "12345678";
    char from[] = "abcd";
    ncopy(to, from, 6);
    assert(!(ncmp(to, "abcd5678", 8)));
}

int ncat(char *left, char *right, int n) {
    while (*left) {
        left++;
    }
    return ncopy(left, right, n);
}

void test_ncat() {
    char left[] = "123\0__";
    char right[] = "abc";
    ncat(left, right, 3);
    assert(!ncmp(left, "123abc", 6));
}

int main() {
    test_ncmp();

    test_ncopy_n_lt();
    test_ncopy_n_eq();
    test_ncopy_n_gt();

    test_ncat();
}
