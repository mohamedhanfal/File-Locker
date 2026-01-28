#include "file_io.h"
#include <stdio.h>

int copy_file_binary(const char *src_path, const char *dst_path) {
    FILE *in = fopen(src_path, "rb");
    if (!in) return 0;

    FILE *out = fopen(dst_path, "wb");
    if (!out) {
        fclose(in);
        return 0;
    }

    unsigned char buffer[64 * 1024]; /* 64 KB */
    size_t nread;

    while ((nread = fread(buffer, 1, sizeof(buffer), in)) > 0) {
        size_t nwritten = fwrite(buffer, 1, nread, out);
        if (nwritten != nread) {
            fclose(in);
            fclose(out);
            return 0;
        }
    }

    if (ferror(in)) {
        fclose(in);
        fclose(out);
        return 0;
    }

    fclose(in);
    fclose(out);
    return 1;
}
