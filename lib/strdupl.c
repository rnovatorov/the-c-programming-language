#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "strncopy.h"

char *strdupl(char *s) {
    size_t len = strlen(s);
    char *dup = malloc(len + 1);
    if (dup == NULL) {
        return NULL;
    }
    strncopy(dup, s, len + 1);
    return dup;
}
