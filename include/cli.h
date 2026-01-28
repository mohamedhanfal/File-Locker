#ifndef CLI_H
#define CLI_H

typedef enum {
    CMD_NONE = 0,
    CMD_PACK,
    CMD_UNPACK,
    CMD_HELP
} Command;

typedef struct {
    Command cmd;
    const char *input_path;
    const char *output_path;
} CliArgs;

int cli_parse(int argc, char **argv, CliArgs *out);
void cli_print_usage(const char *prog);

#endif