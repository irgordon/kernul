/*
 * tests/console/test_interactive_execution_failure_initiation_gate_link.c
 *
 * Link-check for failure acknowledgment-driven initiation gate symbols.
 * Pass condition: successful link.
 */

#include <kernul/interactive_execution_failure_initiation_gate.h>
#include <kernul/interactive_execution_failure_ack_gate.h>
#include <kernul/interactive_execution_transfer_initiation_gate.h>

int main(void)
{
    (void)&interactive_execution_initiation_forbid_on_failure_ack;
    (void)&interactive_execution_initiation_is_permitted_acquire;
    (void)&interactive_execution_failure_ack_acquire;
    (void)&interactive_execution_transfer_initiate;
    return 0;
}
