/*
 * tests/console/test_interactive_selection_link.c
 *
 * Link-check for interactive selection contract symbols.
 * Pass condition: successful link.
 */

#include <kernul/interactive_selection.h>

int main(void)
{
    (void)&interactive_selection_select;
    (void)&interactive_selection_state;
    return 0;
}
