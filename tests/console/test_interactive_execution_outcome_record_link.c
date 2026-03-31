/*
 * tests/console/test_interactive_execution_outcome_record_link.c
 *
 * Link-check for interactive execution outcome record symbols.
 * Pass condition: successful link.
 */

#include <kernul/interactive_execution_outcome_record.h>
#include <kernul/interactive_execution.h>

int main(void)
{
    (void)&interactive_execution_outcome_record_publish_from_transfer;
    (void)&interactive_execution_outcome_record_acquire;
    (void)&interactive_execution_transfer;
    return 0;
}
