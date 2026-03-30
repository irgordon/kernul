/*
 * tests/console/test_interactive_execution_handoff_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/interactive_execution_handoff.h.
 * Pass condition: successful compilation.
 */

#include <kernul/interactive_execution_handoff.h>
#include <kernul/compiler.h>
#include <kernul/interactive_selection_result.h>
#include <kernul/interactive_scheduler_container.h>
#include <kernul/interactive_scheduler_state.h>
#include <kernul/session.h>

static struct interactive_execution_handoff *(*const check_interactive_execution_handoff_materialize)(
    struct session *,
    const struct interactive_selection_result *) =
    interactive_execution_handoff_materialize;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct interactive_execution_handoff handoff = {0};
    struct interactive_selection_result selection = {0};
    struct session session = {0};

    (void)check_interactive_execution_handoff_materialize;

    (void)handoff.session;
    (void)handoff.selection;
    (void)handoff.operand_identity;
    (void)selection.session;
    (void)selection.operand_identity;

    (void)session.execution_handoff;
    (void)session.execution_handoff_live;
    (void)session.scheduler_state;
    (void)session.scheduler_container;
}
