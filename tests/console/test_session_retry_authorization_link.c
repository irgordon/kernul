/*
 * tests/console/test_session_retry_authorization_link.c
 *
 * Link-check for retry authorization dependency boundary symbols.
 * Pass condition: successful link against retry authorization implementation
 * with explicit local stubs for allowed dependency surfaces only.
 */

#include <kernul/session_retry_authorization.h>
#include <kernul/session_retry_policy.h>

session_retry_policy_t
session_get_retry_policy_acquire(const struct session *session)
{
    (void)session;
    return SESSION_RETRY_ALLOWED;
}

bool
session_is_ready_acquire(const struct session *session)
{
    (void)session;
    return true;
}

int main(void)
{
    (void)&session_grant_retry_authorization;
    (void)&session_consume_retry_authorization;
    (void)&session_get_retry_authorization_acquire;
    (void)&session_get_retry_policy_acquire;
    (void)&session_is_ready_acquire;
    return 0;
}
