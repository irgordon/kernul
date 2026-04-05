/*
 * tests/console/test_session_retry_outcome_link.c
 *
 * Link-check for retry outcome dependency boundary symbols.
 * Pass condition: successful link against retry outcome implementation with
 * explicit local stubs for allowed dependency surfaces only.
 */

#include <kernul/session_retry_outcome.h>

bool
session_is_ready_acquire(const struct session *session)
{
    (void)session;
    return true;
}

u32
session_retry_outcome_state_load_acquire(const struct session *session)
{
    (void)session;
    return (u32)SESSION_RETRY_OUTCOME_UNSET;
}

bool
session_retry_outcome_try_record_release(
    struct session *session,
    session_retry_outcome_state_t outcome_state)
{
    (void)session;
    (void)outcome_state;
    return true;
}

u32
session_retry_execution_result_load_published_acquire(const struct session *session)
{
    (void)session;
    return 1U;
}

int main(void)
{
    (void)&session_record_retry_outcome;
    (void)&session_is_ready_acquire;
    (void)&session_retry_outcome_state_load_acquire;
    (void)&session_retry_outcome_try_record_release;
    (void)&session_retry_execution_result_load_published_acquire;
    return 0;
}
