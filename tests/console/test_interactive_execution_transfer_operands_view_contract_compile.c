/*
 * tests/console/test_interactive_execution_transfer_operands_view_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/interactive_execution_transfer_operands_view.h.
 * Pass condition: successful compilation.
 */

#include <kernul/interactive_execution_transfer_operands_view.h>
#include <kernul/compiler.h>
#include <kernul/session.h>

static const struct interactive_execution_transfer_operands_view *(*const
    check_interactive_execution_transfer_operands_view_expose)(
    struct session *,
    const struct interactive_execution_handoff *) =
    interactive_execution_transfer_operands_view_expose;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct interactive_execution_transfer_operands_view view = {0};
    struct interactive_execution_handoff handoff = {0};
    struct session session = {0};

    (void)check_interactive_execution_transfer_operands_view_expose;

    (void)view.session;
    (void)view.handoff;
    (void)view.operand_identity;

    (void)handoff.session;
    (void)handoff.selection;
    (void)handoff.operand_identity;

    (void)session.execution_handoff;
    (void)session.execution_handoff_live;
    (void)session.execution_transfer_operands_view;
    (void)session.execution_transfer_operands_view_live;
}
