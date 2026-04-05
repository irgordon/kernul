/*
 * core/session_retry_outcome.c
 *
 * Immutable single-assignment recording of retry lifecycle outcome.
 *
 * This file records terminal retry outcome exactly once for a ready session
 * using only durable published retry execution result state. It does not
 * execute retries, authorize retries, schedule work, coordinate subsystems,
 * infer policy, or escalate failures.
 */

#include <kernul/session_retry_execution_result.h>
#include <kernul/session_retry_outcome.h>

struct session;

bool
session_is_ready_acquire(const struct session *session);

u32
session_retry_outcome_state_load_acquire(const struct session *session);

bool
session_retry_outcome_try_record_release(
    struct session *session,
    session_retry_outcome_state_t outcome_state);

u32
session_retry_execution_result_load_published_acquire(const struct session *session);

session_retry_outcome_record_result_t
session_record_retry_outcome(struct session *session)
{
    session_retry_outcome_state_t outcome_state;
    u32 published_exec_result;

    if (session == NULL)
        return SESSION_RETRY_OUTCOME_RECORD_FAILED;

    if (!session_is_ready_acquire(session))
        return SESSION_RETRY_OUTCOME_RECORD_NOT_READY;

    if (session_retry_outcome_state_load_acquire(session)
        != (u32)SESSION_RETRY_OUTCOME_UNSET) {
        return SESSION_RETRY_OUTCOME_RECORD_ALREADY_SET;
    }

    published_exec_result = session_retry_execution_result_load_published_acquire(session);
    if (published_exec_result == 0U)
        return SESSION_RETRY_OUTCOME_RECORD_NO_RESULT;

    outcome_state = SESSION_RETRY_OUTCOME_FAILURE;
    if (published_exec_result == ((u32)SESSION_RETRY_EXEC_OK + 1U))
        outcome_state = SESSION_RETRY_OUTCOME_SUCCESS;

    if (!session_retry_outcome_try_record_release(session, outcome_state))
        return SESSION_RETRY_OUTCOME_RECORD_ALREADY_SET;

    return SESSION_RETRY_OUTCOME_RECORD_OK;
}
