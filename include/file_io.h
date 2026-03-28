#ifndef FILE_IO_H
#define FILE_IO_H

#include <stddef.h>

/* Read entire file into memory (caller must free) */
void* read_file_all(const char *filename, size_t *out_size);

/* Write buffer to file */
int write_file_all(const char *filename, const void *data, size_t size);

/* Binary copy: stream large files without loading into memory */
int copy_file_binary(const char *src, const char *dst);

/* Hide file from directory listing (Windows only, no-op on Unix) */
int set_file_hidden(const char *filename);

#endif