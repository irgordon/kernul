/*
 * tests/console/test_interactive_scheduler_state_link.c
 *
 * Link-check for interactive scheduler-state contract symbols.
 * Pass condition: successful link.
 */

#include <kernul/interactive_scheduler_state.h>

int main(void)
{
    (void)&interactive_scheduler_state_create;
    (void)&interactive_scheduler_state_state;
    return 0;
}
