#include <stdio.h>
#include "cli.h"
#include "file_io.h"

int main(int argc, char **argv) {
    CliArgs args;

    if (!cli_parse(argc, argv, &args)) {
        fprintf(stderr, "Invalid arguments.\n");
        cli_print_usage(argv[0]);
        return 2;
    }

    if (args.cmd == CMD_HELP) {
        cli_print_usage(argv[0]);
        return 0;
    }

    if (args.cmd == CMD_PACK) {
        if (!copy_file_binary(args.input_path, args.output_path)) {
            fprintf(stderr, "PACK failed: could not write output.\n");
            return 1;
        }
        printf("PACK ok (temporary: just copied input -> output)\n");
        return 0;
    }

    if (args.cmd == CMD_UNPACK) {
        if (!copy_file_binary(args.input_path, args.output_path)) {
            fprintf(stderr, "UNPACK failed: could not write output.\n");
            return 1;
        }
        printf("UNPACK ok (temporary: just copied input -> output)\n");
        return 0;
    }

    cli_print_usage(argv[0]);
    return 0;
}