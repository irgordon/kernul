/*
 * tests/console/test_session_recovery_authorization_link.c
 *
 * Link-check for recovery authorization dependency boundary symbols.
 * Pass condition: successful link against authorization implementation
 * with explicit local stubs for allowed dependency surfaces only.
 */

#include <kernul/session_recovery_authorization.h>
#include <kernul/session_finalization.h>
#include <kernul/session_recovery_eligibility.h>

bool
session_is_finalized_acquire(const struct session *session)
{
    (void)session;
    return true;
}

session_recovery_eligibility_t
session_get_recovery_eligibility_acquire(const struct session *session)
{
    (void)session;
    return SESSION_RECOVERY_ELIGIBLE;
}

int main(void)
{
    (void)&session_authorize_recovery_attempt;
    (void)&session_consume_recovery_authorization;
    (void)&session_is_recovery_authorized_acquire;
    (void)&session_is_finalized_acquire;
    (void)&session_get_recovery_eligibility_acquire;
    return 0;
}
