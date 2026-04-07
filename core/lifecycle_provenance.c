/*
 * core/lifecycle_provenance.c
 *
 * Read-only lifecycle provenance derived from existing lifecycle facts.
 */

#include <kernul/lifecycle_provenance.h>

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

u64
session_recovery_outcome_timestamp_acquire(const struct session *session);

u32
session_retry_policy_state_acquire(const struct session *session);

u32
session_retry_authorization_state_load_acquire(const struct session *session);

u32
session_retry_execution_result_load_published_acquire(const struct session *session);

u32
session_retry_outcome_state_load_acquire(const struct session *session);

static u64
lifecycle_timestamp_from_bool(bool observed)
{
    if (!observed)
        return 0U;

    return 1U;
}

int
lifecycle_get_provenance(
    const struct session *session,
    lifecycle_provenance_t *out_provenance)
{
    if (session == NULL)
        return LIFECYCLE_PROVENANCE_FAILED;
    if (out_provenance == NULL)
        return LIFECYCLE_PROVENANCE_FAILED;
    if (!session_is_ready_acquire(session))
        return LIFECYCLE_PROVENANCE_NOT_READY;

    out_provenance->finalization_timestamp =
        lifecycle_timestamp_from_bool(session_is_finalized_acquire(session));
    out_provenance->readiness_timestamp =
        lifecycle_timestamp_from_bool(session_is_ready_acquire(session));

    out_provenance->ownership_timestamp =
        (uint64_t)session_owner_id_load_acquire(session);
    out_provenance->reclamation_timestamp =
        lifecycle_timestamp_from_bool(session_reclaimed_load_acquire(session));

    out_provenance->recovery_eligibility_timestamp =
        (uint64_t)session_recovery_eligibility_state_acquire(session);
    out_provenance->recovery_authorization_timestamp =
        (uint64_t)session_recovery_authorization_state_acquire(session);
    out_provenance->recovery_execution_timestamp =
        lifecycle_timestamp_from_bool(session_recovery_execution_completed_acquire(session));
    out_provenance->recovery_outcome_timestamp =
        (uint64_t)session_recovery_outcome_timestamp_acquire(session);

    out_provenance->retry_policy_timestamp =
        (uint64_t)session_retry_policy_state_acquire(session);
    out_provenance->retry_authorization_timestamp =
        (uint64_t)session_retry_authorization_state_load_acquire(session);
    out_provenance->retry_execution_timestamp =
        (uint64_t)session_retry_execution_result_load_published_acquire(session);
    out_provenance->retry_outcome_timestamp =
        (uint64_t)session_retry_outcome_state_load_acquire(session);

    return LIFECYCLE_PROVENANCE_OK;
}
