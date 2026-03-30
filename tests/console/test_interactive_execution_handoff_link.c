/*
 * tests/console/test_interactive_execution_handoff_link.c
 *
 * Link-check for interactive execution handoff contract symbols.
 * Pass condition: successful link.
 */

#include <kernul/interactive_execution_handoff.h>

int main(void)
{
    (void)&interactive_execution_handoff_materialize;
    return 0;
}
