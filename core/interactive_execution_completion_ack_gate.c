/*
 * core/interactive_execution_completion_ack_gate.c
 *
 * Interactive execution completion acknowledgment gate policy surface.
 *
 * This file implements one explicit reaction only:
 * publish a session-owned completion acknowledgment gate on published
 * COMPLETED outcome view observation.
 * No retries, no backoff, no scheduling semantics, and no asynchronous
 * evaluation are implemented here.
 */

#include <kernul/interactive_execution_completion_ack_gate.h>
#include <kernul/interactive_execution_outcome_view.h>
#include <kernul/session.h>

void
interactive_execution_completion_ack_publish_if_completed(struct session *session)
{
    const struct interactive_execution_outcome_view *outcome_view;

    if (session == NULL)
        return;

    outcome_view = interactive_execution_outcome_view_acquire(session);
    if (outcome_view == NULL)
        return;

    if (outcome_view->kind != INTERACTIVE_EXECUTION_OUTCOME_COMPLETED)
        return;

    session->execution_completion_ack_view.published = 1U;

    /*
     * Publish point: release-store is the explicit visibility transition
     * for the session-owned completion acknowledgment gate.
     */
    __atomic_store_n(&session->execution_completion_ack_view_live,
                     1U,
                     __ATOMIC_RELEASE);
}

const struct interactive_execution_completion_ack_view *
interactive_execution_completion_ack_acquire(struct session *session)
{
    if (session == NULL)
        return NULL;

    if (__atomic_load_n(&session->execution_completion_ack_view_live,
                        __ATOMIC_ACQUIRE) != 1U) {
        return NULL;
    }

    return &session->execution_completion_ack_view;
}
