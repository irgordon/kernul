/*
 * core/session_retry_policy.c
 *
 * Immutable declarative retry eligibility publication.
 *
 * This file records retry eligibility meaning only.
 * It does not grant authority, execute retries, schedule work, coordinate, or
 * mutate lifecycle surfaces beyond retry policy storage transition.
 */

#include <kernul/session_retry_policy.h>
#include "session_retry_policy_storage.h"

struct session;

bool
session_is_ready_acquire(const struct session *session);

session_retry_policy_result_t
session_declare_retry_policy(
    struct session *session,
    session_retry_policy_t policy)
{
    if (session == NULL)
        return SESSION_RETRY_POLICY_FAILED;

    if (!session_is_ready_acquire(session))
        return SESSION_RETRY_POLICY_NOT_READY;

    if (policy != SESSION_RETRY_ALLOWED
        && policy != SESSION_RETRY_FORBIDDEN) {
        return SESSION_RETRY_POLICY_FAILED;
    }

    if (!session_retry_policy_try_publish_release(session, policy))
        return SESSION_RETRY_POLICY_ALREADY_DECLARED;

    return SESSION_RETRY_POLICY_OK;
}

session_retry_policy_t
session_get_retry_policy_acquire(const struct session *session)
{
    return (session_retry_policy_t)
        session_retry_policy_state_acquire(session);
}
