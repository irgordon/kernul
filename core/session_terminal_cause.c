/*
 * core/session_terminal_cause.c
 *
 * Terminal session cause classification publication.
 *
 * This file implements one explicit externally triggered publication path
 * only: when a session is already terminal, publish exactly one descriptive
 * terminal cause for that session.
 * No retries, no restart, no cleanup, no scheduling behavior, and no
 * asynchronous inference are implemented here.
 */

#include <kernul/session_terminal_cause.h>
#include <kernul/interactive_execution_completion_ack_gate.h>
#include <kernul/interactive_execution_failure_ack_gate.h>
#include <kernul/session.h>
#include <kernul/session_terminal_state.h>

enum session_terminal_cause
session_get_terminal_cause_acquire(const struct session *session)
{
    if (session == NULL)
        return SESSION_TERMINAL_CAUSE_UNSPECIFIED;
    if (!session_is_ready_acquire(session))
        return SESSION_TERMINAL_CAUSE_UNSPECIFIED;

    return __atomic_load_n(&session->terminal_cause, __ATOMIC_ACQUIRE);
}

void
session_publish_terminal_cause_if_terminal(
    struct session *session,
    enum session_terminal_cause cause)
{
    enum session_terminal_cause current_cause;
    const struct interactive_execution_completion_ack_view *completion_ack;
    const struct interactive_execution_failure_ack_view *failure_ack;
    bool completed_visible;
    bool failed_visible;

    if (session == NULL)
        return;

    /*
     * Terminal-first guard: terminal cause classification is published only
     * after terminal session state is already visible.
     */
    if (!session_is_terminal_acquire(session))
        return;

    if (cause != SESSION_TERMINAL_CAUSE_COMPLETED &&
        cause != SESSION_TERMINAL_CAUSE_FAILED) {
        return;
    }

    completion_ack = interactive_execution_completion_ack_acquire(session);
    failure_ack = interactive_execution_failure_ack_acquire(session);
    completed_visible = completion_ack != NULL;
    failed_visible = failure_ack != NULL;

    if (cause == SESSION_TERMINAL_CAUSE_COMPLETED && !completed_visible)
        return;

    if (cause == SESSION_TERMINAL_CAUSE_FAILED && !failed_visible)
        return;

    current_cause = __atomic_load_n(&session->terminal_cause, __ATOMIC_ACQUIRE);
    if (current_cause != SESSION_TERMINAL_CAUSE_UNSPECIFIED)
        return;

    /*
     * Monotonic one-shot publication point: cause is release-published once
     * for this session lifetime and never reset or overridden.
     *
     * Informational-only contract: this classification is descriptive and must
     * not drive control behavior (initiation, terminal publication, cleanup,
     * retry/restart, scheduling, or selection).
     */
    __atomic_store_n(&session->terminal_cause, cause, __ATOMIC_RELEASE);
}
