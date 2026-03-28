/*
 * tests/console/test_interactive_readiness_link.c
 *
 * Link-check for interactive readiness contract symbols.
 * Pass condition: successful link.
 */

#include <kernul/interactive_readiness.h>

int main(void)
{
    (void)&interactive_readiness_mark_ready;
    (void)&interactive_readiness_state;
    return 0;
}
