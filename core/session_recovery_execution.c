/*
 * core/session_recovery_execution.c
 *
 * Explicit single-attempt recovery execution for finalized authorized sessions.
 *
 * This file executes one recovery attempt only after explicit authorization
 * consumption succeeds. It does not implement retries, scheduling,
 * coordination, or lifecycle mutation.
 */

#include <kernul/session_recovery_execution.h>
#include <kernul/session_finalization.h>
#include <kernul/session_recovery_authorization.h>
#include "session_recovery_execution_storage.h"

/*
 * Recovery execution primitive boundary.
 *
 * If no primitive exists for this phase, execution deterministically fails.
 * The primitive must remain synchronous and single-attempt.
 */
static bool
session_recovery_execute_primitive(struct session *session)
{
    (void)session;
    return false;
}

session_recovery_execution_result_t
session_execute_recovery(struct session *session)
{
    session_recovery_consume_result_t consume_result;
    session_recovery_execution_result_t result;
    bool attempted;

    if (session == NULL)
        return SESSION_RECOVERY_EXEC_FAILED;

    result = SESSION_RECOVERY_EXEC_FAILED;
    attempted = false;

    if (!session_is_finalized_acquire(session))
        result = SESSION_RECOVERY_EXEC_NOT_FINALIZED;
    else {
        consume_result = session_consume_recovery_authorization(session);
        if (consume_result != SESSION_RECOVERY_CONSUME_OK)
            result = SESSION_RECOVERY_EXEC_NOT_AUTHORIZED;
        else {
            attempted = true;
            if (!session_recovery_execute_primitive(session))
                result = SESSION_RECOVERY_EXEC_FAILED;
            else
                result = SESSION_RECOVERY_EXEC_OK;
        }
    }

    if (attempted)
        session_publish_recovery_execution_completion_release(session, result);

    return result;
}
