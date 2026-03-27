/*
 * tests/console/test_interactive_console_link.c
 *
 * Link-check for interactive console path contract symbols.
 * Pass condition: successful link.
 */

#include <kernul/interactive_console.h>

int main(void)
{
    (void)&interactive_console_attach;
    (void)&interactive_console_consumer;
    return 0;
}
