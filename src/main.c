#include <stdio.h>
#include "cli.h"

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
        printf("PACK (not implemented yet)\n");
        printf("  input : %s\n", args.input_path);
        printf("  output: %s\n", args.output_path);
        return 0;
    }

    if (args.cmd == CMD_UNPACK) {
        printf("UNPACK (not implemented yet)\n");
        printf("  input : %s\n", args.input_path);
        printf("  output: %s\n", args.output_path);
        return 0;
    }

    cli_print_usage(argv[0]);
    return 0;
}