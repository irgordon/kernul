/*
 * core/session_retry_authorization_storage.c
 *
 * Session-owned storage access for retry authorization capability state.
 */

#include <kernul/session.h>
#include "session_retry_authorization_storage.h"

u32
session_retry_authorization_state_load_acquire(const struct session *session)
{
    if (session == NULL)
        return (u32)SESSION_RETRY_AUTH_NONE;

    return __atomic_load_n(&session->retry_authorization_state, __ATOMIC_ACQUIRE);
}

void
session_retry_authorization_state_store_release(
    struct session *session,
    session_retry_authorization_state_t state)
{
    if (session == NULL)
        return;

    __atomic_store_n(&session->retry_authorization_state, (u32)state,
                     __ATOMIC_RELEASE);
}

bool
session_retry_authorization_try_grant_release(struct session *session)
{
    u32 expected;

    if (session == NULL)
        return false;

    expected = (u32)SESSION_RETRY_AUTH_NONE;
    return __atomic_compare_exchange_n(&session->retry_authorization_state,
                                       &expected,
                                       (u32)SESSION_RETRY_AUTH_GRANTED,
                                       false,
                                       __ATOMIC_RELEASE,
                                       __ATOMIC_ACQUIRE);
}

bool
session_retry_authorization_try_consume_release(struct session *session)
{
    u32 expected;

    if (session == NULL)
        return false;

    expected = (u32)SESSION_RETRY_AUTH_GRANTED;
    return __atomic_compare_exchange_n(&session->retry_authorization_state,
                                       &expected,
                                       (u32)SESSION_RETRY_AUTH_CONSUMED,
                                       false,
                                       __ATOMIC_RELEASE,
                                       __ATOMIC_ACQUIRE);
}
