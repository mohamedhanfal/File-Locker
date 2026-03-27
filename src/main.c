#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cli.h"
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

    encrypt_xor(data, input_size, password, pwd_len);

    if (!container_write(args->output_file, data, input_size, (uint64_t)input_size)) {
        fprintf(stderr, "Failed to write lock container: %s\n", args->output_file);
        free(data);
        return 1;
    }

    free(data);
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

    if (!write_file_all(args->output_file, payload, payload_size)) {
        fprintf(stderr, "Failed to write output file: %s\n", args->output_file);
        free(payload);
        return 1;
    }

    if (original_size != (uint64_t)payload_size) {
        fprintf(stderr, "Warning: header original size (%llu) != payload size (%zu)\n",
                (unsigned long long)original_size, payload_size);
    }

    free(payload);
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
        printf("File Locker (Phase 1)\n");
        printf("Commands:\n");
        printf("  %s pack <input_file> <locker_file>\n", argv[0]);
        printf("  %s unlock <locker_file> <output_file>\n", argv[0]);
        printf("Notes:\n");
        printf("  - Password is always requested interactively\n");
        printf("  - Phase 1 uses XOR encryption and custom container\n");
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