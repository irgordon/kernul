/*
 * tests/console/test_interactive_activation_link.c
 *
 * Link-check for interactive activation contract symbols.
 * Pass condition: successful link.
 */

#include <kernul/interactive_activation.h>

int main(void)
{
    (void)&interactive_activation_create;
    (void)&interactive_activation_state;
    return 0;
}
