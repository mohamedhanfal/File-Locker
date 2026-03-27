#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cli.h"
#include "../include/compression.h"
#include "../include/container.h"
#include "../include/encryption.h"
#include "../include/file_io.h"

static int run_pack(const CLIArgs *args, const unsigned char *password, size_t pwd_len) {
    size_t input_size = 0;
    unsigned char *data = (unsigned char *)read_file_all(args->input_file, &input_size);
    if (!data) {
        fprintf(stderr, "Failed to read input file: %s\n", args->input_file);
        return 1;
    }

    unsigned char *compressed = NULL;
    size_t compressed_size = compress_buffer(data, input_size, &compressed);
    free(data);
    if (compressed_size == 0 || !compressed) {
        fprintf(stderr, "Compression failed\n");
        return 1;
    }

    encrypt_xor(compressed, compressed_size, password, pwd_len);

    if (!container_write(args->output_file, compressed, compressed_size, (uint64_t)input_size)) {
        fprintf(stderr, "Failed to write lock container: %s\n", args->output_file);
        free(compressed);
        return 1;
    }

    free(compressed);
    printf("Packed successfully: %s -> %s\n", args->input_file, args->output_file);
    return 0;
}

static int run_unlock(const CLIArgs *args, const unsigned char *password, size_t pwd_len) {
    unsigned char *payload = NULL;
    size_t payload_size = 0;
    uint64_t original_size = 0;

    if (!container_read(args->input_file, &payload, &payload_size, &original_size)) {
        fprintf(stderr, "Failed to read lock container (bad format or file): %s\n", args->input_file);
        return 1;
    }

    decrypt_xor(payload, payload_size, password, pwd_len);

    unsigned char *decompressed = NULL;
    size_t out_size = decompress_buffer(payload, payload_size, (size_t)original_size, &decompressed);
    free(payload);
    if (out_size == 0 || !decompressed) {
        fprintf(stderr, "Decompression failed (possibly wrong password or corrupted file)\n");
        return 1;
    }

    if (!write_file_all(args->output_file, decompressed, out_size)) {
        fprintf(stderr, "Failed to write output file: %s\n", args->output_file);
        free(decompressed);
        return 1;
    }

    free(decompressed);
    printf("Unlocked successfully: %s -> %s\n", args->input_file, args->output_file);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s pack <input_file> <locker_file>\n", argv[0]);
        fprintf(stderr, "       %s unlock <locker_file> <output_file>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        printf("File Locker (Phase 4)\n");
        printf("Commands:\n");
        printf("  %s pack <input_file> <locker_file>\n", argv[0]);
        printf("  %s unlock <locker_file> <output_file>\n", argv[0]);
        printf("Pipeline:\n");
        printf("  pack   = compress -> encrypt -> container\n");
        printf("  unlock = container -> decrypt -> decompress\n");
        printf("Password is always requested interactively.\n");
        return 0;
    }

    CLIArgs args;
    if (!parse_cli_args(argc, argv, &args)) {
        fprintf(stderr, "Invalid arguments. Use --help for usage.\n");
        return 1;
    }

    char password[256];
    if (get_password_interactive(password, sizeof(password)) != 0) {
        fprintf(stderr, "Failed to read password\n");
        return 1;
    }

    size_t pwd_len = strlen(password);
    if (pwd_len == 0) {
        fprintf(stderr, "Password cannot be empty\n");
        return 1;
    }

    if (strcmp(args.command, "pack") == 0) {
        return run_pack(&args, (const unsigned char *)password, pwd_len);
    }

    return run_unlock(&args, (const unsigned char *)password, pwd_len);
}