#include "../include/compression.h"

size_t compress_bound(size_t original_size) {
    /* TODO: Replace with miniz_compressBound */
    return original_size + 1024;
}

size_t compress_buffer(const unsigned char *original, size_t original_size,
                       unsigned char **compressed_out) {
    /* TODO: Implement miniz compression */
    return 0;
}

size_t decompress_buffer(const unsigned char *compressed, size_t compressed_size,
                         size_t original_size, unsigned char **decompressed_out) {
    /* TODO: Implement miniz decompression */
    return 0;
}