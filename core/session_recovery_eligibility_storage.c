/*
 * core/session_recovery_eligibility_storage.c
 *
 * Session-owned storage access for recovery eligibility declaration.
 */

#include <kernul/session.h>
#include "session_recovery_eligibility_storage.h"

u32
session_recovery_eligibility_state_acquire(const struct session *session)
{
    if (session == NULL)
        return (u32)SESSION_RECOVERY_ELIGIBILITY_UNSET;

    return __atomic_load_n(&session->recovery_eligibility, __ATOMIC_ACQUIRE);
}

bool
session_recovery_eligibility_try_publish_release(
    struct session *session,
    session_recovery_eligibility_t eligibility)
{
    u32 expected;

    if (session == NULL)
        return false;

    expected = (u32)SESSION_RECOVERY_ELIGIBILITY_UNSET;
    return __atomic_compare_exchange_n(&session->recovery_eligibility,
                                       &expected,
                                       (u32)eligibility,
                                       false,
                                       __ATOMIC_RELEASE,
                                       __ATOMIC_ACQUIRE);
}
