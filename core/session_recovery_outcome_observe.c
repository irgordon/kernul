/*
 * core/session_recovery_outcome_observe.c
 *
 * Read-only recovery outcome observation snapshot.
 */

#include <kernul/session_recovery_outcome_observe.h>
#include "session_recovery_execution_storage.h"
#include "session_recovery_outcome_storage.h"

struct session;

bool
session_is_ready_acquire(const struct session *session);

bool
session_get_recovery_outcome_info_acquire(
    const struct session *session,
    session_recovery_outcome_info_t *out_info)
{
    session_recovery_outcome_info_t info;
    bool completed_before;
    bool completed_after;

    if (session == NULL || out_info == NULL)
        return false;
    if (!session_is_ready_acquire(session))
        return false;

    do {
        completed_before = session_recovery_execution_completed_acquire(session);
        (void)session_recovery_execution_result_acquire(session);
        info.outcome = (session_recovery_outcome_t)
            session_recovery_outcome_state_acquire(session);
        info.outcome_timestamp = session_recovery_outcome_timestamp_acquire(session);
        completed_after = session_recovery_execution_completed_acquire(session);
    } while (completed_before != completed_after);

    info.execution_attempt_count = completed_after ? 1U : 0U;

    *out_info = info;
    return true;
}
