/*
 * core/session_recovery_authorization_storage.c
 *
 * Session-owned storage access for recovery attempt authorization state.
 */

#include <kernul/session.h>
#include "session_recovery_authorization_storage.h"

u32
session_recovery_authorization_state_acquire(const struct session *session)
{
    if (session == NULL)
        return (u32)SESSION_RECOVERY_AUTHORIZATION_UNAUTHORIZED;
    if (!session_is_ready_acquire(session))
        return (u32)SESSION_RECOVERY_AUTHORIZATION_UNAUTHORIZED;

    return __atomic_load_n(&session->recovery_authorization_state,
                           __ATOMIC_ACQUIRE);
}

bool
session_recovery_authorization_try_authorize_release(struct session *session)
{
    u32 expected;

    if (session == NULL)
        return false;

    expected = (u32)SESSION_RECOVERY_AUTHORIZATION_UNAUTHORIZED;
    return __atomic_compare_exchange_n(&session->recovery_authorization_state,
                                       &expected,
                                       (u32)SESSION_RECOVERY_AUTHORIZATION_AUTHORIZED,
                                       false,
                                       __ATOMIC_RELEASE,
                                       __ATOMIC_ACQUIRE);
}

bool
session_recovery_authorization_try_consume_release(struct session *session)
{
    u32 expected;

    if (session == NULL)
        return false;

    expected = (u32)SESSION_RECOVERY_AUTHORIZATION_AUTHORIZED;
    return __atomic_compare_exchange_n(&session->recovery_authorization_state,
                                       &expected,
                                       (u32)SESSION_RECOVERY_AUTHORIZATION_CONSUMED,
                                       false,
                                       __ATOMIC_RELEASE,
                                       __ATOMIC_ACQUIRE);
}
