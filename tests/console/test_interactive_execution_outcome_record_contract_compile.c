/*
 * tests/console/test_interactive_execution_outcome_record_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/interactive_execution_outcome_record.h.
 * Pass condition: successful compilation.
 */

#include <kernul/interactive_execution_outcome_record.h>
#include <kernul/compiler.h>
#include <kernul/interactive_execution.h>
#include <kernul/session.h>

static struct interactive_execution_outcome_record *(*const
    check_interactive_execution_outcome_record_publish_from_transfer)(
    struct session *,
    struct interactive_execution *) =
    interactive_execution_outcome_record_publish_from_transfer;

static const struct interactive_execution_outcome_record *(*const
    check_interactive_execution_outcome_record_acquire)(
    struct session *) =
    interactive_execution_outcome_record_acquire;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct interactive_execution_outcome_record record = {0};
    struct session session = {0};

    (void)check_interactive_execution_outcome_record_publish_from_transfer;
    (void)check_interactive_execution_outcome_record_acquire;

    (void)INTERACTIVE_EXECUTION_OUTCOME_COMPLETED;
    (void)INTERACTIVE_EXECUTION_OUTCOME_FAILED;
    (void)record.kind;
    (void)session.execution_outcome_record;
    (void)session.execution_outcome_record_live;
}
