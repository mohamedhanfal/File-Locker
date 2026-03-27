#include <stdio.h>
#include <stdlib.h>
#include "../include/cli.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s pack <input> <output>\n", argv[0]);
        fprintf(stderr, "       %s unlock <input> <output>\n", argv[0]);
        return 1;
    }

    CLIArgs args;
    if (!parse_cli_args(argc, argv, &args)) {
        fprintf(stderr, "Invalid arguments\n");
        return 1;
    }

    char password[256];
    if (get_password_interactive(password, sizeof(password)) != 0) {
        fprintf(stderr, "Failed to read password\n");
        return 1;
    }

    printf("[TODO] Command: %s\n", args.command);
    printf("       Input:  %s\n", args.input_file);
    printf("       Output: %s\n", args.output_file);

    return 0;
}