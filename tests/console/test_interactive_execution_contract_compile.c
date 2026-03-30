/*
 * tests/console/test_interactive_execution_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/interactive_execution.h.
 * Pass condition: successful compilation.
 */

#include <kernul/interactive_execution.h>
#include <kernul/compiler.h>

static struct interactive_execution *(*const check_interactive_execution_transfer)(
    struct interactive_switch_operands *) =
    interactive_execution_transfer;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct interactive_execution ie = {0};

    (void)check_interactive_execution_transfer;

    (void)INTERACTIVE_EXECUTION_STATE_NONE;
    (void)INTERACTIVE_EXECUTION_STATE_EXECUTED;
    (void)ie.operands;
    (void)ie.consumer_group;
    (void)ie.state;
}
