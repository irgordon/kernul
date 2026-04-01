/*
 * core/interactive_execution_failure_initiation_gate.c
 *
 * Interactive execution failure acknowledgment-driven initiation gate policy.
 *
 * This file implements one explicit externally triggered reaction only:
 * when failure acknowledgment is published for a session, irreversibly forbid
 * future execution initiation for that session.
 * No retries, no fallback, no scheduling semantics, no time/fairness/priority
 * semantics, and no asynchronous evaluation are implemented here.
 */

#include <kernul/interactive_execution_failure_initiation_gate.h>
#include <kernul/interactive_execution_failure_ack_gate.h>
#include <kernul/session.h>

void
interactive_execution_initiation_forbid_on_failure_ack(struct session *session)
{
    const struct interactive_execution_failure_ack_view *ack_view;

    if (session == NULL)
        return;

    ack_view = interactive_execution_failure_ack_acquire(session);
    if (ack_view == NULL)
        return;

    (void)ack_view;

    /*
     * Monotonic publication point: once failure acknowledgment is visible,
     * release-store initiates an irreversible transition to initiation
     * forbidden for this session lifetime.
     */
    __atomic_store_n(&session->execution_initiation_permitted,
                     0U,
                     __ATOMIC_RELEASE);
}

bool
interactive_execution_initiation_is_permitted_acquire(const struct session *session)
{
    if (session == NULL)
        return false;

    return __atomic_load_n(&session->execution_initiation_permitted,
                           __ATOMIC_ACQUIRE) == 1U;
}
