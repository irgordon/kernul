/*
 * core/session_retry_execution.c
 *
 * Explicit single-attempt retry execution for ready authorized sessions.
 *
 * This file executes one retry attempt only after explicit authorization
 * consumption succeeds. It does not implement retry loops, scheduling,
 * coordination, policy inference, or lifecycle storage mutation.
 */

#include <kernul/session_retry_execution.h>
#include <kernul/session_retry_execution_result.h>
#include <kernul/session_retry_authorization.h>
#include "session_retry_authorization_storage.h"

struct session;

bool
session_is_ready_acquire(const struct session *session);

/*
 * Retry execution primitive boundary.
 *
 * If no primitive exists for this phase, execution deterministically fails.
 * The primitive must remain synchronous and single-attempt.
 */
static bool
session_retry_execute_primitive(struct session *session)
{
    (void)session;
    return false;
}

session_retry_execution_result_t
session_execute_retry(struct session *session)
{
    session_retry_execution_result_t execution_result;

    if (session == NULL)
        return SESSION_RETRY_EXEC_FAILED;

    if (!session_is_ready_acquire(session))
        return SESSION_RETRY_EXEC_NOT_READY;

    if (!session_retry_authorization_try_consume_release(session))
        return SESSION_RETRY_EXEC_NOT_AUTHORIZED;

    execution_result = SESSION_RETRY_EXEC_OK;
    if (!session_retry_execute_primitive(session))
        execution_result = SESSION_RETRY_EXEC_FAILED;

    (void)session_publish_retry_execution_result(session, execution_result);
    return execution_result;
}
