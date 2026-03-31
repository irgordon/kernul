/*
 * tests/console/test_interactive_execution_outcome_view_link.c
 *
 * Link-check for interactive execution outcome view symbols.
 * Pass condition: successful link.
 */

#include <kernul/interactive_execution_outcome_view.h>

int main(void)
{
    (void)&interactive_execution_outcome_view_acquire;
    return 0;
}
