#include <stdio.h>
#include <string.h>
#include "cli.h"

void cli_print_usage(const char *prog) {
    printf("Usage:\n");
    printf("  %s pack <input_file> <output_file>\n", prog);
    printf("  %s unpack <input_file> <output_file>\n", prog);
    printf("  %s --help\n", prog);
}

int cli_parse(int argc, char **argv, CliArgs *out) {
    if (!out) return 0;

    out->cmd = CMD_NONE;
    out->input_path = NULL;
    out->output_path = NULL;

    if (argc < 2) {
        out->cmd = CMD_HELP;
        return 1;
    }

    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        out->cmd = CMD_HELP;
        return 1;
    }

    if (strcmp(argv[1], "pack") == 0) {
        if (argc != 4) return 0;
        out->cmd = CMD_PACK;
        out->input_path = argv[2];
        out->output_path = argv[3];
        return 1;
    }

    if (strcmp(argv[1], "unpack") == 0) {
        if (argc != 4) return 0;
        out->cmd = CMD_UNPACK;
        out->input_path = argv[2];
        out->output_path = argv[3];
        return 1;
    }

    return 0;
}
