/*
 * core/interactive_execution_failure_ack_gate.c
 *
 * Interactive execution failure acknowledgment gate policy surface.
 *
 * This file implements one explicit reaction only:
 * publish a session-owned failure acknowledgment gate on published FAILED
 * outcome view observation.
 * No retries, no backoff, no scheduling semantics, and no asynchronous
 * evaluation are implemented here.
 */

#include <kernul/interactive_execution_failure_ack_gate.h>
#include <kernul/interactive_execution_outcome_view.h>
#include <kernul/session.h>

void
interactive_execution_failure_ack_publish_if_failed(struct session *session)
{
    const struct interactive_execution_outcome_view *outcome_view;

    if (session == NULL)
        return;

    outcome_view = interactive_execution_outcome_view_acquire(session);
    if (outcome_view == NULL)
        return;

    if (outcome_view->kind != INTERACTIVE_EXECUTION_OUTCOME_FAILED)
        return;

    session->execution_failure_ack_view.published = 1U;

    /*
     * Publish point: release-store is the explicit visibility transition
     * for the session-owned failure acknowledgment gate.
     */
    __atomic_store_n(&session->execution_failure_ack_view_live,
                     1U,
                     __ATOMIC_RELEASE);
}

const struct interactive_execution_failure_ack_view *
interactive_execution_failure_ack_acquire(struct session *session)
{
    if (session == NULL)
        return NULL;

    if (__atomic_load_n(&session->execution_failure_ack_view_live,
                        __ATOMIC_ACQUIRE) != 1U) {
        return NULL;
    }

    return &session->execution_failure_ack_view;
}
