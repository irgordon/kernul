/*
 * core/lifecycle_introspection.c
 *
 * Read-only lifecycle introspection.
 */

#include <kernul/lifecycle_introspection.h>

struct session;

bool
session_is_ready_acquire(const struct session *session);

bool
session_is_finalized_acquire(const struct session *session);

u64
session_owner_id_load_acquire(const struct session *session);

bool
session_reclaimed_load_acquire(const struct session *session);

u32
session_recovery_eligibility_state_acquire(const struct session *session);

u32
session_recovery_authorization_state_acquire(const struct session *session);

bool
session_recovery_execution_completed_acquire(const struct session *session);

int
session_recovery_execution_result_acquire(const struct session *session);

u32
session_recovery_outcome_state_acquire(const struct session *session);

u32
session_retry_policy_state_acquire(const struct session *session);

u32
session_retry_authorization_state_load_acquire(const struct session *session);

u32
session_retry_execution_result_load_published_acquire(const struct session *session);

u32
session_retry_outcome_state_load_acquire(const struct session *session);

lifecycle_introspect_result_t
lifecycle_introspect(
    const struct session *session,
    lifecycle_snapshot_t *out_snapshot)
{
    u32 retry_exec_result;

    if (session == NULL)
        return LIFECYCLE_INTROSPECT_FAILED;
    if (out_snapshot == NULL)
        return LIFECYCLE_INTROSPECT_FAILED;
    if (!session_is_ready_acquire(session))
        return LIFECYCLE_INTROSPECT_NOT_READY;

    out_snapshot->finalized = session_is_finalized_acquire(session);
    out_snapshot->ready = session_is_ready_acquire(session);

    out_snapshot->owner_id = (uint64_t)session_owner_id_load_acquire(session);
    out_snapshot->reclaimed = session_reclaimed_load_acquire(session);

    out_snapshot->recovery_eligible =
        session_recovery_eligibility_state_acquire(session) == 1U;
    out_snapshot->recovery_authorized =
        session_recovery_authorization_state_acquire(session) == 1U;
    out_snapshot->recovery_executed =
        session_recovery_execution_completed_acquire(session);
    out_snapshot->recovery_execution_result =
        session_recovery_execution_result_acquire(session);
    out_snapshot->recovery_outcome =
        (int)session_recovery_outcome_state_acquire(session);

    out_snapshot->retry_policy = (int)session_retry_policy_state_acquire(session);
    out_snapshot->retry_authorized =
        session_retry_authorization_state_load_acquire(session) == 1U;

    retry_exec_result = session_retry_execution_result_load_published_acquire(session);
    out_snapshot->retry_executed = retry_exec_result != 0U;
    out_snapshot->retry_execution_result = (int)retry_exec_result;
    out_snapshot->retry_outcome =
        (int)session_retry_outcome_state_load_acquire(session);

    return LIFECYCLE_INTROSPECT_OK;
}
