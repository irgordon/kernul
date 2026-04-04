/*
 * tests/console/test_session_recovery_execution_link.c
 *
 * Link-check for recovery execution dependency boundary symbols.
 * Pass condition: successful link against execution implementation with
 * explicit local stubs for allowed dependency surfaces only.
 */

#include <kernul/session_recovery_execution.h>
#include <kernul/session_finalization.h>
#include <kernul/session_recovery_authorization.h>

bool
session_is_finalized_acquire(const struct session *session)
{
    (void)session;
    return true;
}

bool
session_is_recovery_authorized_acquire(const struct session *session)
{
    (void)session;
    return true;
}

session_recovery_consume_result_t
session_consume_recovery_authorization(struct session *session)
{
    (void)session;
    return SESSION_RECOVERY_CONSUME_OK;
}

int main(void)
{
    (void)&session_execute_recovery;
    (void)&session_is_finalized_acquire;
    (void)&session_is_recovery_authorized_acquire;
    (void)&session_consume_recovery_authorization;
    return 0;
}
