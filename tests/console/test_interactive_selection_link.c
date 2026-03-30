/*
 * tests/console/test_interactive_selection_link.c
 *
 * Link-check for interactive selection contract symbols.
 * Pass condition: successful link.
 */

#include <kernul/interactive_selection.h>
#include <kernul/interactive_scheduler_container.h>
#include <kernul/interactive_scheduler_state.h>

int main(void)
{
    (void)&interactive_selection_select;
    (void)&interactive_selection_result_expose;
    (void)&interactive_selection_state;
    (void)&interactive_scheduler_state_create;
    (void)&interactive_scheduler_container_materialize;
    return 0;
}
