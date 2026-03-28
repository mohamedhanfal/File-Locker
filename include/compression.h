#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <stddef.h>

/* Compress buffer using zlib */
/* Returns compressed size, or 0 on failure */
size_t compress_buffer(const unsigned char *original, size_t original_size,
                       unsigned char **compressed_out);

/* Decompress buffer */
/* Returns original size on success, 0 on failure */
size_t decompress_buffer(const unsigned char *compressed, size_t compressed_size,
                         size_t original_size, unsigned char **decompressed_out);

/* Helper: get max possible compressed size */
size_t compress_bound(size_t original_size);

#endif