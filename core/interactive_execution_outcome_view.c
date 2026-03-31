/*
 * core/interactive_execution_outcome_view.c
 *
 * Interactive execution outcome view exposure.
 *
 * This file exposes passive acquire observation of session-owned published
 * outcome storage only.
 * No allocation, no mutation, no retry logic, no fallback behavior, and no
 * policy semantics are implemented here.
 */

#include <kernul/interactive_execution_outcome_view.h>
#include <kernul/session.h>

const struct interactive_execution_outcome_view *
interactive_execution_outcome_view_acquire(struct session *session)
{
    if (session == NULL)
        return NULL;

    /*
     * Acquire point: consume outcome publication marker visibility before
     * alias-view observation of session-owned outcome storage.
     */
    if (__atomic_load_n(&session->execution_outcome_record_live,
                        __ATOMIC_ACQUIRE) != 1U) {
        return NULL;
    }

    return (const struct interactive_execution_outcome_view *)
        &session->execution_outcome_record;
}
