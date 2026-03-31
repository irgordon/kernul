/*
 * tests/console/test_interactive_selection_handoff_binding_link.c
 *
 * Link-check for selection-handoff binding contract symbols.
 * Pass condition: successful link.
 */

#include <kernul/interactive_selection_handoff_binding.h>
#include <kernul/interactive_selection.h>
#include <kernul/interactive_execution_handoff.h>

int main(void)
{
    (void)&interactive_selection_handoff_bind;
    (void)&interactive_selection_result_expose;
    (void)&interactive_execution_handoff_materialize;
    return 0;
}
