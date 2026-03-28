/*
 * tests/console/test_interactive_dispatch_link.c
 *
 * Link-check for interactive dispatch contract symbols.
 * Pass condition: successful link.
 */

#include <kernul/interactive_dispatch.h>

int main(void)
{
    (void)&interactive_dispatch_select;
    (void)&interactive_dispatch_state;
    return 0;
}
