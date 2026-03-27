#ifndef CONTAINER_H
#define CONTAINER_H

#include <stddef.h>
#include <stdint.h>

#define CONTAINER_MAGIC "FLK1"
#define CONTAINER_VERSION 1

typedef struct {
    char magic[4];              // "FLK1"
    uint32_t version;           // 1
    uint32_t flags;             // 0 for now
    uint64_t payload_size;      // Encrypted+Compressed data size
    uint64_t original_size;     // Original file size (for verification)
} FlkHeader;

/* Write container: header + encrypted/compressed payload */
int container_write(const char *filename, const unsigned char *payload, size_t payload_size,
                    uint64_t original_size);

/* Read container: extract header and payload */
int container_read(const char *filename, unsigned char **payload_out, size_t *payload_size_out,
                   uint64_t *original_size_out);

#endif