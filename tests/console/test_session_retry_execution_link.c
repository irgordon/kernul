/*
 * tests/console/test_session_retry_execution_link.c
 *
 * Link-check for retry execution dependency boundary symbols.
 * Pass condition: successful link against retry execution implementation with
 * explicit local stubs for allowed dependency surfaces only.
 */

#include <kernul/session_retry_execution.h>

bool
session_is_ready_acquire(const struct session *session)
{
    (void)session;
    return true;
}

int main(void)
{
    (void)&session_execute_retry;
    (void)&session_is_ready_acquire;
    return 0;
}
