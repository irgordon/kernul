/*
 * tests/console/test_interactive_execution_link.c
 *
 * Link-check for interactive execution transfer contract symbols.
 * Pass condition: successful link.
 */

#include <kernul/interactive_execution.h>

int main(void)
{
    (void)&interactive_execution_transfer;
    return 0;
}
