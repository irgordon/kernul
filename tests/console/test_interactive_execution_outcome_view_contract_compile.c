/*
 * tests/console/test_interactive_execution_outcome_view_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/interactive_execution_outcome_view.h.
 * Pass condition: successful compilation.
 */

#include <kernul/interactive_execution_outcome_view.h>
#include <kernul/compiler.h>
#include <kernul/session.h>

static const struct interactive_execution_outcome_view *(*const
    check_interactive_execution_outcome_view_acquire)(
    struct session *) =
    interactive_execution_outcome_view_acquire;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct interactive_execution_outcome_view view = {0};
    struct session session = {0};

    (void)check_interactive_execution_outcome_view_acquire;

    (void)view.kind;
    (void)session.execution_outcome_record;
    (void)session.execution_outcome_record_live;
}
