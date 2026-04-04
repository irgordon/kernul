/*
 * core/session_finalization.c
 *
 * Terminal-guarded session finalization marker publication.
 *
 * This file provides one explicit externally-triggered publication path only:
 * when a session is already terminal, publish exactly one finalized marker for
 * that session.
 * No cleanup, no retry/restart, no scheduling behavior, and no asynchronous
 * propagation are implemented here.
 */

#include <kernul/session_finalization.h>
#include <kernul/session.h>
#include <kernul/session_terminal_state.h>

void
session_publish_finalized_if_terminal(struct session *session)
{
    u32 expected;

    if (session == NULL)
        return;

    if (!session_is_terminal_acquire(session))
        return;

    expected = 0U;
    (void)__atomic_compare_exchange_n(&session->finalized_published,
                                      &expected,
                                      1U,
                                      false,
                                      __ATOMIC_RELEASE,
                                      __ATOMIC_ACQUIRE);
}

bool
session_is_finalized_acquire(const struct session *session)
{
    if (session == NULL)
        return false;
    if (!session_is_ready_acquire(session))
        return false;

    return __atomic_load_n(&session->finalized_published, __ATOMIC_ACQUIRE)
        == 1U;
}
