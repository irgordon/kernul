/*
 * tests/console/test_interactive_execution_outcome_aggregate_link.c
 *
 * Link-check for interactive execution outcome aggregate symbols.
 * Pass condition: successful link.
 */

#include <kernul/interactive_execution_outcome_aggregate.h>

int main(void)
{
    (void)&interactive_execution_outcome_aggregate_acquire;
    return 0;
}
