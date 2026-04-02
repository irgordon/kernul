/*
 * tests/console/test_session_terminal_cause_link.c
 *
 * Link-check for terminal cause classification symbols.
 * Pass condition: successful link.
 */

#include <kernul/session_terminal_cause.h>
#include <kernul/session_terminal_state.h>
#include <kernul/interactive_execution_failure_ack_gate.h>
#include <kernul/interactive_execution_completion_ack_gate.h>

int main(void)
{
    (void)&session_get_terminal_cause_acquire;
    (void)&session_publish_terminal_cause_if_terminal;
    (void)&session_is_terminal_acquire;
    (void)&interactive_execution_failure_ack_acquire;
    (void)&interactive_execution_completion_ack_acquire;
    return 0;
}
