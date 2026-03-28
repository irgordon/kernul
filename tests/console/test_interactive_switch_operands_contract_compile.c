/*
 * tests/console/test_interactive_switch_operands_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/interactive_switch_operands.h.
 * Pass condition: successful compilation.
 */

#include <kernul/interactive_switch_operands.h>
#include <kernul/compiler.h>

static struct interactive_switch_operands *(*const
    check_interactive_switch_operands_prepare)(
    struct interactive_execution_target *) =
    interactive_switch_operands_prepare;
static u32 (*const check_interactive_switch_operands_state)(
    const struct interactive_switch_operands *) =
    interactive_switch_operands_state;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct interactive_switch_operands iso = {0};

    (void)check_interactive_switch_operands_prepare;
    (void)check_interactive_switch_operands_state;

    (void)INTERACTIVE_SWITCH_OPERANDS_STATE_NONE;
    (void)INTERACTIVE_SWITCH_OPERANDS_STATE_PREPARED;
    (void)iso.target;
    (void)iso.consumer_group;
    (void)iso.from;
    (void)iso.to;
    (void)iso.state;
}
