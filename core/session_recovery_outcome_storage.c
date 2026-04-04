/*
 * core/session_recovery_outcome_storage.c
 *
 * Session-owned storage access for immutable recovery outcome recording.
 */

#include <kernul/session.h>
#include "session_recovery_outcome_storage.h"

u32
session_recovery_outcome_state_acquire(const struct session *session)
{
    if (session == NULL)
        return (u32)SESSION_RECOVERY_NOT_ATTEMPTED;
    if (!session_is_ready_acquire(session))
        return (u32)SESSION_RECOVERY_NOT_ATTEMPTED;

    return __atomic_load_n(&session->recovery_outcome_state, __ATOMIC_ACQUIRE);
}

bool
session_recovery_outcome_try_publish_release(
    struct session *session,
    session_recovery_outcome_t outcome)
{
    u32 expected;

    if (session == NULL)
        return false;

    expected = (u32)SESSION_RECOVERY_NOT_ATTEMPTED;
    return __atomic_compare_exchange_n(&session->recovery_outcome_state,
                                       &expected,
                                       (u32)outcome,
                                       false,
                                       __ATOMIC_RELEASE,
                                       __ATOMIC_ACQUIRE);
}
