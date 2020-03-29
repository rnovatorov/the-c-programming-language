int strncopy(char *dst, char *src, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (!(*dst++ = *src++)) {
            break;
        }
    }
    return i;
}
