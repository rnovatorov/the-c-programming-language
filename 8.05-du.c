#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_PATH 1024

void du(char *fname) {
    struct stat finfo;
    if (lstat(fname, &finfo) < 0) {
        fprintf(stderr, "stat: %s: %s\n", fname, strerror(errno));
        return;
    }

    if (!S_ISDIR(finfo.st_mode)) {
        printf("%8lu %s\n", finfo.st_size, fname);
        return;
    }

    DIR *dir = opendir(fname);
    if (dir == NULL) {
        fprintf(stderr, "opendir: %s: %s\n", fname, strerror(errno));
        return;
    }

    struct dirent *dentry;
    while ((dentry = readdir(dir))) {
        if (!strcmp(dentry->d_name, ".") || !strcmp(dentry->d_name, "..")) {
            continue;
        }

        char path[MAX_PATH];
        snprintf(path, sizeof(path), "%s/%s", fname, dentry->d_name);
        du(path);
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        du(".");
        return EXIT_SUCCESS;
    }
    for (int i = 1; i < argc; i++) {
        du(argv[i]);
    }
    return EXIT_SUCCESS;
}
