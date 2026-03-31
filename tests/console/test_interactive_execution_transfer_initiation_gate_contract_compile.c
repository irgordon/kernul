/*
 * tests/console/test_interactive_execution_transfer_initiation_gate_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/interactive_execution_transfer_initiation_gate.h.
 * Pass condition: successful compilation.
 */

#include <kernul/interactive_execution_transfer_initiation_gate.h>
#include <kernul/compiler.h>
#include <kernul/interactive_execution_transfer_operands_view.h>
#include <kernul/session.h>

static struct interactive_execution *(*const
    check_interactive_execution_transfer_initiate)(
    struct session *,
    const struct interactive_execution_transfer_operands_view *) =
    interactive_execution_transfer_initiate;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct interactive_execution_transfer_operands_view view = {0};
    struct session session = {0};

    (void)check_interactive_execution_transfer_initiate;
    (void)view.session;
    (void)view.handoff;
    (void)view.operand_identity;
    (void)session.execution_transfer_operands_view;
}
