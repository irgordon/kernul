/*
 * tests/console/test_interactive_execution_transfer_initiation_gate_link.c
 *
 * Link-check for transfer initiation gate contract symbols.
 * Pass condition: successful link.
 */

#include <kernul/interactive_execution_transfer_initiation_gate.h>
#include <kernul/interactive_execution_transfer_operands_view.h>
#include <kernul/interactive_execution.h>

int main(void)
{
    (void)&interactive_execution_transfer_initiate;
    (void)&interactive_execution_transfer_operands_view_expose;
    (void)&interactive_execution_transfer;
    return 0;
}
