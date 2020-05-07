#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void die(char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    fprintf(stderr, "cat: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");

    va_end(args);
    exit(EXIT_FAILURE);
}

void fcopy(int dst, int src, char buf[], size_t bufsize) {
    for (;;) {
        ssize_t r = read(src, buf, bufsize);
        if (r < 0) {
            die("read: %s", strerror(errno));
        }
        if (r == 0) {
            return;
        }

        ssize_t w = write(dst, buf, r);
        if (w < 0) {
            die("write: %s", strerror(errno));
        }
        if (w < r) {
            die("tried to write %lo bytes but written only %lo bytes", r, w);
        }
    }
}

int main(int argc, char *argv[]) {
    char buf[BUFSIZ];

    if (argc == 1) {
        fcopy(STDOUT_FILENO, STDIN_FILENO, buf, BUFSIZ);
        return EXIT_SUCCESS;
    }

    for (size_t i = 1; i < argc; i++) {
        int fd;
        if ((fd = open(argv[i], O_RDONLY)) < 0) {
            die("open: %s: %s", argv[i], strerror(errno));
        }

        fcopy(STDOUT_FILENO, fd, buf, BUFSIZ);

        if (close(fd) < 0) {
            die("close: %s: %s", argv[i], strerror(errno));
        }
    }
    return EXIT_SUCCESS;
}
