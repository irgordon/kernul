/*
 * tests/console/test_interactive_switch_operands_link.c
 *
 * Link-check for interactive switch-operands contract symbols.
 * Pass condition: successful link.
 */

#include <kernul/interactive_switch_operands.h>

int main(void)
{
    (void)&interactive_switch_operands_prepare;
    (void)&interactive_switch_operands_state;
    return 0;
}
