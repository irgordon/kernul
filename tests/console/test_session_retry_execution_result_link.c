/*
 * tests/console/test_session_retry_execution_result_link.c
 *
 * Link-check for retry execution result publication dependency boundary
 * symbols.
 * Pass condition: successful link against publication implementation with
 * explicit local stubs for allowed dependency surfaces only.
 */

#include <kernul/session_retry_execution_result.h>

u32
session_retry_execution_result_state_load_acquire(const struct session *session)
{
    (void)session;
    return (u32)SESSION_RETRY_EXEC_RESULT_UNSET;
}

bool
session_retry_execution_result_try_publish_release(
    struct session *session,
    session_retry_execution_result_t exec_result)
{
    (void)session;
    (void)exec_result;
    return true;
}

bool
session_is_ready_acquire(const struct session *session)
{
    (void)session;
    return true;
}

int main(void)
{
    (void)&session_publish_retry_execution_result;
    (void)&session_get_retry_execution_result_state_acquire;
    (void)&session_retry_execution_result_state_load_acquire;
    (void)&session_retry_execution_result_try_publish_release;
    (void)&session_is_ready_acquire;
    return 0;
}
