/*
 * tests/terminal/test_controlling_terminal_link.c
 *
 * Link-check for controlling terminal attachment contract symbols.
 * Pass condition: successful link.
 */

#include <kernul/controlling_terminal.h>

int main(void)
{
    (void)&controlling_terminal_attach;
    (void)&controlling_terminal_get;
    return 0;
}
