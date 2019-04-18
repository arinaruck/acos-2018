#define _GNU_SOURCE 1
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void writedown(int len, int fd, const char *text) {
    int write_sum = 0;
    int write_res = 0;
    while (write_sum < len && write_res != -1) {
        write_sum += write_res;
        write_res = write(fd, text + write_sum, len - write_sum);
    }
    if (write_res == -1) {
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    char *dir = getenv("XDG_RUNTIME_DIR");
    if (!dir) dir = getenv("TMPDIR");
    if (!dir) dir = "/tmp";
    char* path = (char*)calloc(strlen(dir) + 30, 1);
    int fd;
    do {
        sprintf(path, "%s/s-%d.py", dir, rand());
        fd = open(path, O_CREAT | O_EXCL | O_RDWR , S_IRUSR | S_IXUSR);
    } while (fd < 0);
    const char *shbng = "#!/usr/bin/env python3\nprint(";
    const char mul = '*';
    const char *ending = "1)\n";
    writedown(strlen(shbng), fd, shbng);
    for (size_t i = 1; i < argc; ++i) {
        writedown(strlen(argv[i]), fd, argv[i]);
        writedown(1, fd, &mul);
    }
    writedown(strlen(ending), fd, ending);
    if (close(fd) < 0) {
        return 1;
    }
    fd = open(path, O_RDONLY);
    if (fd < 0) {
        return 1;
    }
    char* empty_argv[] = {"randname", NULL};
    char* empty_env[] = {NULL};
    fexecve(fd, empty_argv, empty_env);
    return(1);
}
