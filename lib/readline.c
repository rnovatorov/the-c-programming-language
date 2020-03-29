#include <stdio.h>

// Reads a line from stream into buf.
//
// The buffer is null-terminated and includes a newline character,
// if one was found.
//
// Returns a number of characters read, including the delimiter character,
// but not including the terminating '\0'.
//
// Result is negated if the delimiter was not found after n characters.
int readline(char buf[], int n, FILE *stream) {
    int i = 0;

    while (i < n - 1) {
        char c = fgetc(stream);

        if (c == EOF) {
            buf[i] = '\0';
            return i;
        }

        buf[i++] = c;

        if (c == '\n') {
            buf[i] = '\0';
            return i;
        }
    }

    buf[i] = '\0';
    return -i;
}
