#include <stdio.h>
#include <stdlib.h>
#include "../include/file_io.h"
#ifdef _WIN32
#include <windows.h>
#endif

void* read_file_all(const char *filename, size_t *out_size) {
    if (!filename || !out_size) return NULL;

    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("fopen");
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (size < 0) {
        fclose(f);
        return NULL;
    }

    if (size == 0) {
        *out_size = 0;
        fclose(f);
        return malloc(1);
    }

    void *data = malloc((size_t)size);
    if (!data) {
        fclose(f);
        return NULL;
    }

    size_t n = fread(data, 1, size, f);
    fclose(f);

    if (n != (size_t)size) {
        free(data);
        return NULL;
    }

    *out_size = (size_t)size;
    return data;
}

int write_file_all(const char *filename, const void *data, size_t size) {
    if (!filename || (!data && size > 0)) return 0;

    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("fopen");
        return 0;
    }

    size_t n = fwrite(data, 1, size, f);
    fclose(f);

    return n == size;
}

int set_file_hidden(const char *filename) {
    if (!filename) return 0;

#ifdef _WIN32
    DWORD attrs = GetFileAttributesA(filename);
    if (attrs == INVALID_FILE_ATTRIBUTES) {
        fprintf(stderr, "Warning: Could not get file attributes for %s\n", filename);
        return 0;
    }

    if (!SetFileAttributesA(filename, attrs | FILE_ATTRIBUTE_HIDDEN)) {
        fprintf(stderr, "Warning: Could not set file hidden attribute for %s\n", filename);
        return 0;
    }
    return 1;
#else
    (void)filename;  /* Suppress unused parameter warning on Unix */
    return 1;  /* No-op on Unix systems */
#endif
}

int copy_file_binary(const char *src, const char *dst) {
    if (!src || !dst) return 0;

    FILE *in = fopen(src, "rb");
    if (!in) {
        perror("fopen (input)");
        return 0;
    }

    FILE *out = fopen(dst, "wb");
    if (!out) {
        perror("fopen (output)");
        fclose(in);
        return 0;
    }

    unsigned char buf[65536];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
        if (fwrite(buf, 1, n, out) != n) {
            perror("fwrite");
            fclose(in);
            fclose(out);
            return 0;
        }
    }

    fclose(in);
    fclose(out);
    return 1;
}