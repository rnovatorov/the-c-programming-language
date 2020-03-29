#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "lib/strncopy.h"

int cmpch(char a, char b) { return (a < b) - (b < a); }

void test_cmpch() {
    assert(cmpch('b', 'a') < 0);
    assert(cmpch('a', 'a') == 0);
    assert(cmpch('a', 'b') > 0);
}

int ncmp(char *a, char *b, int n) {
    int delta = cmpch(*a, *b);
    int i = 0;
    while (i < n && !delta && *a) {
        delta = cmpch(*a, *b);
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

void test_ncopy_n_lt() {
    char to[] = "12345678";
    char from[] = "abcd";
    strncopy(to, from, 2);
    assert(!(ncmp(to, "ab345678", 8)));
}

void test_ncopy_n_eq() {
    char to[] = "12345678";
    char from[] = "abcd";
    strncopy(to, from, 4);
    assert(!(ncmp(to, "abcd5678", 8)));
}

void test_ncopy_n_gt() {
    char to[] = "12345678";
    char from[] = "abcd";
    strncopy(to, from, 6);
    assert(!(ncmp(to, "abcd5678", 8)));
}

int ncat(char *left, char *right, int n) {
    while (*left) {
        left++;
    }
    return strncopy(left, right, n);
}

void test_ncat() {
    char left[] = "123\0__";
    char right[] = "abc";
    ncat(left, right, 3);
    assert(!ncmp(left, "123abc", 6));
}

int main() {
    test_cmpch();

    test_ncmp();

    test_ncopy_n_lt();
    test_ncopy_n_eq();
    test_ncopy_n_gt();

    test_ncat();
}
