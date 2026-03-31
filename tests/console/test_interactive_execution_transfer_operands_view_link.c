/*
 * tests/console/test_interactive_execution_transfer_operands_view_link.c
 *
 * Link-check for interactive execution transfer operands view contract symbols.
 * Pass condition: successful link.
 */

#include <kernul/interactive_execution_transfer_operands_view.h>

int main(void)
{
    (void)&interactive_execution_transfer_operands_view_expose;
    return 0;
}
