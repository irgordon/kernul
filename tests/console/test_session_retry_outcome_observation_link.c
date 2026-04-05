/*
 * tests/console/test_session_retry_outcome_observation_link.c
 *
 * Link-check for retry outcome observation dependency boundary symbols.
 * Pass condition: successful link against observation implementation with
 * explicit local stubs for allowed dependency surfaces only.
 */

#include <kernul/session_retry_outcome_observation.h>

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
    return (u32)SESSION_RETRY_OUTCOME_SUCCESS;
}

int main(void)
{
    (void)&session_observe_retry_outcome;
    (void)&session_is_ready_acquire;
    (void)&session_retry_outcome_state_load_acquire;
    return 0;
}
