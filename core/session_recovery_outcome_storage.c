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
    u64 expected_timestamp;
    u32 expected;

    if (session == NULL)
        return false;

    expected_timestamp = 0U;
    if (!__atomic_compare_exchange_n(&session->recovery_outcome_timestamp,
                                     &expected_timestamp,
                                     1U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        return false;
    }

    expected = (u32)SESSION_RECOVERY_NOT_ATTEMPTED;
    if (!__atomic_compare_exchange_n(&session->recovery_outcome_state,
                                     &expected,
                                     (u32)outcome,
                                     false,
                                     __ATOMIC_RELEASE,
                                     __ATOMIC_ACQUIRE))
        return false;

    return true;
}

u64
session_recovery_outcome_timestamp_acquire(const struct session *session)
{
    if (session == NULL)
        return 0U;
    if (!session_is_ready_acquire(session))
        return 0U;

    return __atomic_load_n(&session->recovery_outcome_timestamp, __ATOMIC_ACQUIRE);
}
