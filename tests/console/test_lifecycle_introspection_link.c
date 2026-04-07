/*
 * tests/console/test_lifecycle_introspection_link.c
 *
 * Link-check for lifecycle introspection dependency boundary symbols.
 * Pass condition: successful link against introspection implementation with
 * explicit local stubs for allowed dependency surfaces only.
 */

#include <kernul/lifecycle_introspection.h>

bool
session_is_ready_acquire(const struct session *session)
{
    (void)session;
    return true;
}

bool
session_is_finalized_acquire(const struct session *session)
{
    (void)session;
    return true;
}

u64
session_owner_id_load_acquire(const struct session *session)
{
    (void)session;
    return 7U;
}

bool
session_reclaimed_load_acquire(const struct session *session)
{
    (void)session;
    return false;
}

u32
session_recovery_eligibility_state_acquire(const struct session *session)
{
    (void)session;
    return 1U;
}

u32
session_recovery_authorization_state_acquire(const struct session *session)
{
    (void)session;
    return 1U;
}

bool
session_recovery_execution_completed_acquire(const struct session *session)
{
    (void)session;
    return true;
}

int
session_recovery_execution_result_acquire(const struct session *session)
{
    (void)session;
    return 0;
}

u32
session_recovery_outcome_state_acquire(const struct session *session)
{
    (void)session;
    return 1U;
}

u32
session_retry_policy_state_acquire(const struct session *session)
{
    (void)session;
    return 1U;
}

u32
session_retry_authorization_state_load_acquire(const struct session *session)
{
    (void)session;
    return 1U;
}

u32
session_retry_execution_result_load_published_acquire(const struct session *session)
{
    (void)session;
    return 1U;
}

u32
session_retry_outcome_state_load_acquire(const struct session *session)
{
    (void)session;
    return 1U;
}

int main(void)
{
    (void)&lifecycle_introspect;
    (void)&session_is_ready_acquire;
    (void)&session_is_finalized_acquire;
    (void)&session_owner_id_load_acquire;
    (void)&session_reclaimed_load_acquire;
    (void)&session_recovery_eligibility_state_acquire;
    (void)&session_recovery_authorization_state_acquire;
    (void)&session_recovery_execution_completed_acquire;
    (void)&session_recovery_execution_result_acquire;
    (void)&session_recovery_outcome_state_acquire;
    (void)&session_retry_policy_state_acquire;
    (void)&session_retry_authorization_state_load_acquire;
    (void)&session_retry_execution_result_load_published_acquire;
    (void)&session_retry_outcome_state_load_acquire;
    return 0;
}
