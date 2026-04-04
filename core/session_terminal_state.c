/*
 * core/session_terminal_state.c
 *
 * Initiation-forbidden-driven terminal session state publication.
 *
 * This file provides one explicit externally triggered reaction only:
 * when execution initiation is observed as forbidden for a session,
 * publish terminal session state for that session.
 * No retries, no restart, no cleanup, no scheduling behavior, and no
 * asynchronous propagation are implemented here.
 */

#include <kernul/session_terminal_state.h>
#include <kernul/interactive_execution_failure_initiation_gate.h>
#include <kernul/session.h>

void
session_publish_terminal_state_if_initiation_forbidden(struct session *session)
{
    if (session == NULL)
        return;

    if (interactive_execution_initiation_is_permitted_acquire(session))
        return;

    /*
     * Monotonic publication point: once initiation is observed as forbidden,
     * terminal state is release-published and is irreversible for this
     * session lifetime.
     */
    __atomic_store_n(&session->terminal_state_published,
                     1U,
                     __ATOMIC_RELEASE);
}

bool
session_is_terminal_acquire(const struct session *session)
{
    if (session == NULL)
        return false;
    if (!session_is_ready_acquire(session))
        return false;

    return __atomic_load_n(&session->terminal_state_published,
                           __ATOMIC_ACQUIRE) == 1U;
}
