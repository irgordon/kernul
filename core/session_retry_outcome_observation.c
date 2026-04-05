/*
 * core/session_retry_outcome_observation.c
 *
 * Read-only retry outcome observation.
 */

#include <kernul/session_retry_outcome_observation.h>

struct session;

bool
session_is_ready_acquire(const struct session *session);

u32
session_retry_outcome_state_load_acquire(const struct session *session);

session_retry_outcome_observation_result_t
session_observe_retry_outcome(
    const struct session *session,
    session_retry_outcome_state_t *out_outcome)
{
    if (session == NULL)
        return SESSION_RETRY_OUTCOME_OBS_FAILED;
    if (out_outcome == NULL)
        return SESSION_RETRY_OUTCOME_OBS_FAILED;
    if (!session_is_ready_acquire(session))
        return SESSION_RETRY_OUTCOME_OBS_NOT_READY;

    *out_outcome = (session_retry_outcome_state_t)
        session_retry_outcome_state_load_acquire(session);
    return SESSION_RETRY_OUTCOME_OBS_OK;
}
