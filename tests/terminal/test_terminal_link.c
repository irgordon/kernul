/*
 * tests/terminal/test_terminal_link.c
 *
 * Link-check for terminal and PTY contract symbols.
 * Pass condition: successful link.
 */

#include <kernul/terminal.h>

int main(void)
{
    (void)&terminal_create;
    (void)&terminal_id;
    (void)&pty_master_create;
    (void)&pty_master_id;
    (void)&pty_pair_create;
    (void)&pty_pair_id;
    return 0;
}
