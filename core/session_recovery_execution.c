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

    if (session == NULL)
        return SESSION_RECOVERY_EXEC_FAILED;

    if (!session_is_finalized_acquire(session))
        return SESSION_RECOVERY_EXEC_NOT_FINALIZED;

    if (!session_is_recovery_authorized_acquire(session))
        return SESSION_RECOVERY_EXEC_NOT_AUTHORIZED;

    consume_result = session_consume_recovery_authorization(session);
    if (consume_result != SESSION_RECOVERY_CONSUME_OK)
        return SESSION_RECOVERY_EXEC_NOT_AUTHORIZED;

    if (!session_recovery_execute_primitive(session))
        return SESSION_RECOVERY_EXEC_FAILED;

    return SESSION_RECOVERY_EXEC_OK;
}
