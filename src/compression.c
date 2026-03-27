#include <limits.h>
#include <stdlib.h>

#include "../include/compression.h"
#include <zlib.h>

size_t compress_bound(size_t original_size) {
    if (original_size > (size_t)ULONG_MAX) {
        return 0;
    }
    return (size_t)compressBound((uLong)original_size);
}

size_t compress_buffer(const unsigned char *original, size_t original_size,
                       unsigned char **compressed_out) {
    if (!original || !compressed_out || original_size == 0) {
        return 0;
    }

    size_t bound = compress_bound(original_size);
    if (bound == 0 || bound > (size_t)ULONG_MAX) {
        return 0;
    }

    unsigned char *compressed = (unsigned char *)malloc(bound);
    if (!compressed) {
        return 0;
    }

    uLongf dst_len = (uLongf)bound;
    int status = compress2(compressed, &dst_len, original, (uLong)original_size, Z_BEST_COMPRESSION);
    if (status != Z_OK) {
        free(compressed);
        return 0;
    }

    *compressed_out = compressed;
    return (size_t)dst_len;
}

size_t decompress_buffer(const unsigned char *compressed, size_t compressed_size,
                         size_t original_size, unsigned char **decompressed_out) {
    if (!compressed || !decompressed_out || compressed_size == 0 || original_size == 0) {
        return 0;
    }

    if (compressed_size > (size_t)ULONG_MAX || original_size > (size_t)ULONG_MAX) {
        return 0;
    }

    unsigned char *decompressed = (unsigned char *)malloc(original_size);
    if (!decompressed) {
        return 0;
    }

    uLongf dst_len = (uLongf)original_size;
    int status = uncompress(decompressed, &dst_len, compressed, (uLong)compressed_size);
    if (status != Z_OK || (size_t)dst_len != original_size) {
        free(decompressed);
        return 0;
    }

    *decompressed_out = decompressed;
    return (size_t)dst_len;
}