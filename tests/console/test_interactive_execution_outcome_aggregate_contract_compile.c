/*
 * tests/console/test_interactive_execution_outcome_aggregate_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/interactive_execution_outcome_aggregate.h.
 * Pass condition: successful compilation.
 */

#include <kernul/interactive_execution_outcome_aggregate.h>
#include <kernul/compiler.h>
#include <kernul/session.h>

static struct interactive_execution_outcome_aggregate (*const
    check_interactive_execution_outcome_aggregate_acquire)(
    struct session *) =
    interactive_execution_outcome_aggregate_acquire;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct interactive_execution_outcome_aggregate aggregate = {0};
    struct session session = {0};

    (void)check_interactive_execution_outcome_aggregate_acquire;

    (void)aggregate.completed_count;
    (void)aggregate.failed_count;
    (void)session.execution_outcome_record;
    (void)session.execution_outcome_record_live;
}
