#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/container.h"

int container_write(const char *filename, const unsigned char *payload, size_t payload_size,
                    uint64_t original_size) {
    if (!filename || !payload) {
        return 0;
    }

    FILE *out = fopen(filename, "wb");
    if (!out) {
        perror("fopen");
        return 0;
    }

    FlkHeader hdr;
    memcpy(hdr.magic, CONTAINER_MAGIC, sizeof(hdr.magic));
    hdr.version = CONTAINER_VERSION;
    hdr.flags = 0;
    hdr.payload_size = (uint64_t)payload_size;
    hdr.original_size = original_size;

    if (fwrite(&hdr, sizeof(hdr), 1, out) != 1) {
        fclose(out);
        return 0;
    }

    if (payload_size > 0 && fwrite(payload, 1, payload_size, out) != payload_size) {
        fclose(out);
        return 0;
    }

    fclose(out);
    return 1;
}

int container_read(const char *filename, unsigned char **payload_out, size_t *payload_size_out,
                   uint64_t *original_size_out) {
    if (!filename || !payload_out || !payload_size_out || !original_size_out) {
        return 0;
    }

    *payload_out = NULL;
    *payload_size_out = 0;
    *original_size_out = 0;

    FILE *in = fopen(filename, "rb");
    if (!in) {
        perror("fopen");
        return 0;
    }

    FlkHeader hdr;
    if (fread(&hdr, sizeof(hdr), 1, in) != 1) {
        fclose(in);
        return 0;
    }

    if (memcmp(hdr.magic, CONTAINER_MAGIC, sizeof(hdr.magic)) != 0 || hdr.version != CONTAINER_VERSION) {
        fclose(in);
        return 0;
    }

    if (hdr.payload_size > (uint64_t)SIZE_MAX) {
        fclose(in);
        return 0;
    }

    size_t payload_size = (size_t)hdr.payload_size;
    unsigned char *payload = NULL;
    if (payload_size > 0) {
        payload = (unsigned char *)malloc(payload_size);
        if (!payload) {
            fclose(in);
            return 0;
        }

        if (fread(payload, 1, payload_size, in) != payload_size) {
            free(payload);
            fclose(in);
            return 0;
        }
    }

    fclose(in);

    *payload_out = payload;
    *payload_size_out = payload_size;
    *original_size_out = hdr.original_size;
    return 1;
}