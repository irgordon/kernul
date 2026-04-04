/*
 * core/session_recovery_authorization.c
 *
 * Explicit single-attempt authorization and consumption for session recovery.
 *
 * This file grants/consumes capability only.
 * It does not perform recovery, retries, scheduling, coordination, or
 * lifecycle mutation.
 */

#include <kernul/session_recovery_authorization.h>
#include <kernul/session_finalization.h>
#include <kernul/session_recovery_eligibility.h>
#include "session_recovery_authorization_storage.h"

session_recovery_authorization_result_t
session_authorize_recovery_attempt(struct session *session)
{
    if (session == NULL)
        return SESSION_RECOVERY_AUTH_FAILED;

    if (!session_is_finalized_acquire(session))
        return SESSION_RECOVERY_AUTH_NOT_FINALIZED;

    if (session_get_recovery_eligibility_acquire(session)
        != SESSION_RECOVERY_ELIGIBLE) {
        return SESSION_RECOVERY_AUTH_NOT_ELIGIBLE;
    }

    if (!session_recovery_authorization_try_authorize_release(session))
        return SESSION_RECOVERY_AUTH_ALREADY_USED;

    return SESSION_RECOVERY_AUTH_OK;
}

session_recovery_consume_result_t
session_consume_recovery_authorization(struct session *session)
{
    u32 state;

    if (session == NULL)
        return SESSION_RECOVERY_CONSUME_FAILED;

    state = session_recovery_authorization_state_acquire(session);
    if (state == (u32)SESSION_RECOVERY_AUTHORIZATION_AUTHORIZED) {
        if (session_recovery_authorization_try_consume_release(session))
            return SESSION_RECOVERY_CONSUME_OK;

        state = session_recovery_authorization_state_acquire(session);
    }

    if (state == (u32)SESSION_RECOVERY_AUTHORIZATION_CONSUMED)
        return SESSION_RECOVERY_CONSUME_ALREADY_CONSUMED;

    return SESSION_RECOVERY_CONSUME_NOT_AUTHORIZED;
}

bool
session_is_recovery_authorized_acquire(const struct session *session)
{
    return session_recovery_authorization_state_acquire(session)
        == (u32)SESSION_RECOVERY_AUTHORIZATION_AUTHORIZED;
}
