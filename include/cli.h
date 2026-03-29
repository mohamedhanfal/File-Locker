#ifndef CLI_H
#define CLI_H

#include <stddef.h>

typedef struct {
    const char *command;        // "lock" or "unlock"
    const char *input_file;
    const char *output_file;
    int use_password;           // 0 = prompt, 1 = provided (not used in interactive mode)
    const char *password;       // Only if --password provided (future expansion)
} CLIArgs;

/* Parse command line arguments */
int parse_cli_args(int argc, char *argv[], CLIArgs *args);

/* Get password from user (interactive, hidden input) */
int get_password_interactive(char *password, size_t max_len);

#endif