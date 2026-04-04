/*
 * core/session_recovery_eligibility.c
 *
 * Declarative finalized-session recovery eligibility publication.
 *
 * This file provides one informational declaration path:
 * record whether a finalized session is eligible for recovery consideration.
 * No recovery behavior, retry behavior, scheduling behavior, coordination, or
 * lifecycle mutation is implemented here.
 */

#include <kernul/session_recovery_eligibility.h>
#include <kernul/session_finalization.h>
#include "session_recovery_eligibility_storage.h"

session_recovery_eligibility_result_t
session_declare_recovery_eligibility(
    struct session *session,
    session_recovery_eligibility_t eligibility)
{
    if (session == NULL)
        return SESSION_RECOVERY_ELIGIBILITY_FAILED;

    if (!session_is_finalized_acquire(session))
        return SESSION_RECOVERY_ELIGIBILITY_NOT_FINALIZED;

    if (eligibility != SESSION_RECOVERY_ELIGIBLE
        && eligibility != SESSION_RECOVERY_INELIGIBLE) {
        return SESSION_RECOVERY_ELIGIBILITY_FAILED;
    }

    if (!session_recovery_eligibility_try_publish_release(session, eligibility))
        return SESSION_RECOVERY_ELIGIBILITY_ALREADY_SET;

    return SESSION_RECOVERY_ELIGIBILITY_OK;
}

session_recovery_eligibility_t
session_get_recovery_eligibility_acquire(
    const struct session *session)
{
    if (session == NULL)
        return SESSION_RECOVERY_ELIGIBILITY_UNSET;

    return (session_recovery_eligibility_t)
        session_recovery_eligibility_state_acquire(session);
}
