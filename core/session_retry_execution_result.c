/*
 * core/session_retry_execution_result.c
 *
 * Immutable single-assignment publication of retry execution result state.
 *
 * This file publishes terminal retry execution result exactly once for a ready
 * session. It does not execute retries, grant authorization, infer policy,
 * schedule work, or coordinate subsystems.
 */

#include <kernul/session_retry_execution_result.h>
#include "session_retry_execution_result_storage.h"

struct session;

bool
session_is_ready_acquire(const struct session *session);

session_retry_execution_result_publish_result_t
session_publish_retry_execution_result(
    struct session *session,
    session_retry_execution_result_t exec_result)
{
    if (session == NULL)
        return SESSION_RETRY_EXEC_RESULT_PUBLISH_FAILED;

    if (!session_is_ready_acquire(session))
        return SESSION_RETRY_EXEC_RESULT_PUBLISH_NOT_READY;

    if (session_retry_execution_result_state_load_acquire(session)
        != (u32)SESSION_RETRY_EXEC_RESULT_UNSET) {
        return SESSION_RETRY_EXEC_RESULT_PUBLISH_ALREADY_SET;
    }

    if (!session_retry_execution_result_try_publish_release(session, exec_result)) {
        return SESSION_RETRY_EXEC_RESULT_PUBLISH_ALREADY_SET;
    }

    return SESSION_RETRY_EXEC_RESULT_PUBLISH_OK;
}

session_retry_execution_result_state_t
session_get_retry_execution_result_state_acquire(
    const struct session *session)
{
    if (session == NULL)
        return SESSION_RETRY_EXEC_RESULT_UNSET;
    if (!session_is_ready_acquire(session))
        return SESSION_RETRY_EXEC_RESULT_UNSET;

    return (session_retry_execution_result_state_t)
        session_retry_execution_result_state_load_acquire(session);
}
