/*
 * tests/console/test_interactive_execution_target_link.c
 *
 * Link-check for interactive execution-target contract symbols.
 * Pass condition: successful link.
 */

#include <kernul/interactive_execution_target.h>

int main(void)
{
    (void)&interactive_execution_prepare;
    (void)&interactive_execution_target_state;
    return 0;
}
