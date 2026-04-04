/*
 * core/session_recovery_outcome.c
 *
 * Immutable declarative recording of recovery execution outcome.
 *
 * This file records exactly one lifecycle fact for a recovery execution
 * attempt after completion is observed.
 * It does not retry, schedule, coordinate, authorize, or mutate lifecycle
 * surfaces beyond the outcome storage transition.
 */

#include <kernul/session_recovery_outcome.h>
#include "session_recovery_execution_storage.h"
#include "session_recovery_outcome_storage.h"

session_recovery_outcome_result_t
session_record_recovery_outcome(
    struct session *session,
    session_recovery_outcome_t outcome)
{
    if (session == NULL)
        return SESSION_RECOVERY_OUTCOME_FAILED;

    if (!session_recovery_execution_completed_acquire(session))
        return SESSION_RECOVERY_OUTCOME_NOT_READY;

    if (outcome != SESSION_RECOVERY_SUCCEEDED
        && outcome != SESSION_RECOVERY_FAILED) {
        return SESSION_RECOVERY_OUTCOME_FAILED;
    }

    if (session_recovery_execution_result_acquire(session) == SESSION_RECOVERY_EXEC_OK
        && outcome != SESSION_RECOVERY_SUCCEEDED) {
        return SESSION_RECOVERY_OUTCOME_FAILED;
    }

    if (session_recovery_execution_result_acquire(session) != SESSION_RECOVERY_EXEC_OK
        && outcome != SESSION_RECOVERY_FAILED) {
        return SESSION_RECOVERY_OUTCOME_FAILED;
    }

    if (session_recovery_outcome_state_acquire(session)
        != (u32)SESSION_RECOVERY_NOT_ATTEMPTED) {
        return SESSION_RECOVERY_OUTCOME_ALREADY_RECORDED;
    }

    if (!session_recovery_outcome_try_publish_release(session, outcome))
        return SESSION_RECOVERY_OUTCOME_ALREADY_RECORDED;

    return SESSION_RECOVERY_OUTCOME_OK;
}

session_recovery_outcome_t
session_get_recovery_outcome_acquire(const struct session *session)
{
    if (session == NULL)
        return SESSION_RECOVERY_NOT_ATTEMPTED;

    return (session_recovery_outcome_t)
        session_recovery_outcome_state_acquire(session);
}
