/*
 * core/interactive_execution_outcome_aggregate.c
 *
 * Interactive execution outcome per-session aggregation.
 *
 * This file provides passive aggregation over one session outcome view.
 * No allocation, no mutation, no persistence, no retries/backoff, and no
 * policy semantics are implemented here.
 */

#include <kernul/interactive_execution_outcome_aggregate.h>
#include <kernul/interactive_execution_outcome_view.h>

struct interactive_execution_outcome_aggregate
interactive_execution_outcome_aggregate_acquire(struct session *session)
{
    struct interactive_execution_outcome_aggregate aggregate = {0};
    const struct interactive_execution_outcome_view *view;

    view = interactive_execution_outcome_view_acquire(session);
    if (view == NULL)
        return aggregate;

    if (view->kind == INTERACTIVE_EXECUTION_OUTCOME_COMPLETED)
        aggregate.completed_count = 1U;
    else if (view->kind == INTERACTIVE_EXECUTION_OUTCOME_FAILED)
        aggregate.failed_count = 1U;

    return aggregate;
}
