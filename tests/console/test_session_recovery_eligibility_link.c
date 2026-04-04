/*
 * tests/console/test_session_recovery_eligibility_link.c
 *
 * Link-check for recovery eligibility dependency boundary symbols.
 * Pass condition: successful link against recovery eligibility implementation
 * with explicit local stubs for allowed dependency surfaces only.
 */

#include <kernul/session_recovery_eligibility.h>
#include <kernul/session_finalization.h>

bool
session_is_finalized_acquire(const struct session *session)
{
    (void)session;
    return true;
}

int main(void)
{
    (void)&session_declare_recovery_eligibility;
    (void)&session_get_recovery_eligibility_acquire;
    (void)&session_is_finalized_acquire;
    return 0;
}
