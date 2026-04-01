/*
 * tests/console/test_interactive_execution_failure_initiation_gate_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/interactive_execution_failure_initiation_gate.h.
 * Pass condition: successful compilation.
 */

#include <kernul/interactive_execution_failure_initiation_gate.h>
#include <kernul/compiler.h>
#include <kernul/session.h>

static void (*const
    check_interactive_execution_initiation_forbid_on_failure_ack)(
    struct session *) =
    interactive_execution_initiation_forbid_on_failure_ack;

static bool (*const
    check_interactive_execution_initiation_is_permitted_acquire)(
    const struct session *) =
    interactive_execution_initiation_is_permitted_acquire;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct session session = {0};

    (void)check_interactive_execution_initiation_forbid_on_failure_ack;
    (void)check_interactive_execution_initiation_is_permitted_acquire;
    (void)session.execution_initiation_permitted;
}
