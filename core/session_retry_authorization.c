/*
 * core/session_retry_authorization.c
 *
 * Explicit single-use retry authorization capability.
 *
 * This file grants/consumes capability only.
 * It does not execute retries, schedule retries, coordinate retries, or
 * mutate lifecycle surfaces beyond retry authorization state.
 */

#include <kernul/session_retry_authorization.h>
#include <kernul/session_retry_policy.h>
#include "session_retry_authorization_storage.h"

struct session;

bool
session_is_ready_acquire(const struct session *session);

session_retry_authorization_result_t
session_grant_retry_authorization(struct session *session)
{
    if (session == NULL)
        return SESSION_RETRY_AUTH_FAILED;

    if (!session_is_ready_acquire(session))
        return SESSION_RETRY_AUTH_NOT_READY;

    if (session_get_retry_policy_acquire(session) != SESSION_RETRY_ALLOWED)
        return SESSION_RETRY_AUTH_POLICY_FORBIDS;

    if (session_retry_authorization_state_load_acquire(session)
        != (u32)SESSION_RETRY_AUTH_NONE) {
        return SESSION_RETRY_AUTH_ALREADY_GRANTED;
    }

    if (!session_retry_authorization_try_grant_release(session))
        return SESSION_RETRY_AUTH_ALREADY_GRANTED;

    return SESSION_RETRY_AUTH_OK;
}

session_retry_authorization_result_t
session_consume_retry_authorization(struct session *session)
{
    if (session == NULL)
        return SESSION_RETRY_AUTH_FAILED;

    if (!session_is_ready_acquire(session))
        return SESSION_RETRY_AUTH_NOT_READY;

    if (session_retry_authorization_state_load_acquire(session)
        != (u32)SESSION_RETRY_AUTH_GRANTED) {
        return SESSION_RETRY_AUTH_NOT_GRANTED;
    }

    if (!session_retry_authorization_try_consume_release(session))
        return SESSION_RETRY_AUTH_NOT_GRANTED;

    return SESSION_RETRY_AUTH_OK;
}

session_retry_authorization_state_t
session_get_retry_authorization_acquire(const struct session *session)
{
    if (session == NULL)
        return SESSION_RETRY_AUTH_NONE;

    if (!session_is_ready_acquire(session))
        return SESSION_RETRY_AUTH_NONE;

    return (session_retry_authorization_state_t)
        session_retry_authorization_state_load_acquire(session);
}
