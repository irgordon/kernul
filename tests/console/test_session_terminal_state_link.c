/*
 * tests/console/test_session_terminal_state_link.c
 *
 * Link-check for terminal session state publication symbols.
 * Pass condition: successful link.
 */

#include <kernul/session_terminal_state.h>
#include <kernul/interactive_execution_failure_initiation_gate.h>

int main(void)
{
    (void)&session_publish_terminal_state_if_initiation_forbidden;
    (void)&session_is_terminal_acquire;
    (void)&interactive_execution_initiation_is_permitted_acquire;
    return 0;
}
