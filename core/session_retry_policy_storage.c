/*
 * core/session_retry_policy_storage.c
 *
 * Session-owned storage access for immutable retry policy declaration.
 */

#include <kernul/session.h>
#include "session_retry_policy_storage.h"

u32
session_retry_policy_state_acquire(const struct session *session)
{
    if (session == NULL)
        return (u32)SESSION_RETRY_UNSET;
    if (!session_is_ready_acquire(session))
        return (u32)SESSION_RETRY_UNSET;

    return __atomic_load_n(&session->retry_policy_state, __ATOMIC_ACQUIRE);
}

bool
session_retry_policy_try_publish_release(
    struct session *session,
    session_retry_policy_t policy)
{
    u32 expected;

    if (session == NULL)
        return false;

    expected = (u32)SESSION_RETRY_UNSET;
    return __atomic_compare_exchange_n(&session->retry_policy_state,
                                       &expected,
                                       (u32)policy,
                                       false,
                                       __ATOMIC_RELEASE,
                                       __ATOMIC_ACQUIRE);
}
