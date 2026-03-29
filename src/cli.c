#include <stdio.h>
#include <string.h>
#include "../include/cli.h"

#ifdef _WIN32
#include <conio.h>  // Windows: _getch()
#include <windows.h>
#else
#include <termios.h>  // Unix: tcgetattr/tcsetattr
#include <unistd.h>
#endif

int parse_cli_args(int argc, char *argv[], CLIArgs *args) {
    if (argc < 4) return 0;

    args->command = argv[1];
    args->input_file = argv[2];
    args->output_file = argv[3];
    args->use_password = 0;
    args->password = NULL;

    if (strcmp(args->command, "lock") != 0 && strcmp(args->command, "unlock") != 0) {
        return 0;
    }

    return 1;
}

int get_password_interactive(char *password, size_t max_len) {
    if (!password || max_len < 1) return -1;

    printf("Enter password: ");
    fflush(stdout);

#ifdef _WIN32
    /* Windows: read hidden password using _getch() */
    size_t pos = 0;
    int ch;
    while ((ch = _getch()) != '\r' && pos < max_len - 1) {
        if (ch == '\b') {  // Backspace
            if (pos > 0) pos--;
        } else if (ch >= 32 && ch <= 126) {  // Printable ASCII
            password[pos++] = ch;
            putchar('*');  // Echo asterisk
        }
    }
    password[pos] = '\0';
    putchar('\n');
    fflush(stdout);
    return 0;
#else
    /* Unix/Linux: read hidden password using termios */
    struct termios old_term, new_term;
    tcgetattr(STDIN_FILENO, &old_term);
    new_term = old_term;
    new_term.c_lflag &= ~ECHO;  // Disable echo
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);

    if (fgets(password, max_len, stdin) == NULL) {
        tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
        return -1;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
    putchar('\n');

    // Remove trailing newline
    size_t len = strlen(password);
    if (len > 0 && password[len - 1] == '\n') {
        password[len - 1] = '\0';
    }

    return 0;
#endif
}