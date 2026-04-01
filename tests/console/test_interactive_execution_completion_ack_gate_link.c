/*
 * tests/console/test_interactive_execution_completion_ack_gate_link.c
 *
 * Link-check for completion acknowledgment gate policy symbols.
 * Pass condition: successful link.
 */

#include <kernul/interactive_execution_completion_ack_gate.h>
#include <kernul/interactive_execution_outcome_view.h>

int main(void)
{
    (void)&interactive_execution_completion_ack_publish_if_completed;
    (void)&interactive_execution_completion_ack_acquire;
    (void)&interactive_execution_outcome_view_acquire;
    return 0;
}
