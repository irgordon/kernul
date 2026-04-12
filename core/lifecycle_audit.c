/*
 * core/lifecycle_audit.c
 *
 * Deterministic, snapshot-scoped lifecycle audit report generation.
 */

#include <stddef.h>

#include <kernul/lifecycle_audit.h>
#include <kernul/lifecycle_freeze_baseline.h>

static bool
lifecycle_audit_structurally_usable(void)
{
    if (LIFECYCLE_FREEZE_VERSION == 0U)
        return false;
    if (sizeof(lifecycle_snapshot_t) != LIFECYCLE_STATE_SIZE)
        return false;
    if (offsetof(lifecycle_snapshot_t, finalized) != LIFECYCLE_STATE_FINALIZED_OFFSET)
        return false;
    if (offsetof(lifecycle_snapshot_t, ready) != LIFECYCLE_STATE_READY_OFFSET)
        return false;
    if (offsetof(lifecycle_snapshot_t, recovery_outcome) != RECOVERY_OUTCOME_OFFSET)
        return false;
    if (offsetof(lifecycle_snapshot_t, retry_outcome) != RETRY_OUTCOME_OFFSET)
        return false;
    if (sizeof(lifecycle_provenance_t) != LIFECYCLE_PROVENANCE_SIZE)
        return false;
    if (offsetof(lifecycle_provenance_t, recovery_outcome_timestamp)
        != LIFECYCLE_PROVENANCE_RECOVERY_OUTCOME_TIMESTAMP_OFFSET) {
        return false;
    }
    if (offsetof(lifecycle_provenance_t, retry_outcome_timestamp)
        != LIFECYCLE_PROVENANCE_RETRY_OUTCOME_TIMESTAMP_OFFSET) {
        return false;
    }

    return true;
}

static bool
lifecycle_audit_provenance_absent_state_valid(
    const lifecycle_snapshot_t *snapshot,
    const lifecycle_provenance_t *provenance)
{
    if (provenance->readiness_timestamp == 0U)
        return false;

    if (snapshot->finalized && provenance->finalization_timestamp == 0U)
        return false;
    if (!snapshot->finalized && provenance->finalization_timestamp != 0U)
        return false;

    if (snapshot->reclaimed && provenance->reclamation_timestamp == 0U)
        return false;
    if (!snapshot->reclaimed && provenance->reclamation_timestamp != 0U)
        return false;

    if (snapshot->recovery_executed && provenance->recovery_execution_timestamp == 0U)
        return false;
    if (!snapshot->recovery_executed && provenance->recovery_execution_timestamp != 0U)
        return false;

    if (snapshot->recovery_outcome == 0 && provenance->recovery_outcome_timestamp != 0U)
        return false;
    if (snapshot->recovery_outcome != 0 && provenance->recovery_outcome_timestamp == 0U)
        return false;

    if (snapshot->retry_executed && provenance->retry_execution_timestamp == 0U)
        return false;
    if (!snapshot->retry_executed && provenance->retry_execution_timestamp != 0U)
        return false;

    if (snapshot->retry_outcome == 0 && provenance->retry_outcome_timestamp != 0U)
        return false;
    if (snapshot->retry_outcome != 0 && provenance->retry_outcome_timestamp == 0U)
        return false;

    return true;
}

lifecycle_audit_result_t
lifecycle_generate_audit_report(
    const lifecycle_snapshot_t *snapshot,
    lifecycle_verify_result_t verify_result,
    const lifecycle_provenance_t *provenance,
    lifecycle_assert_result_t assertion,
    lifecycle_audit_report_t *out_report)
{
    if (snapshot == NULL)
        return LIFECYCLE_AUDIT_FAILED;
    if (provenance == NULL)
        return LIFECYCLE_AUDIT_FAILED;
    if (out_report == NULL)
        return LIFECYCLE_AUDIT_FAILED;
    if (!snapshot->ready)
        return LIFECYCLE_AUDIT_FAILED;
    if (!lifecycle_audit_structurally_usable())
        return LIFECYCLE_AUDIT_FAILED;
    if (!lifecycle_audit_provenance_absent_state_valid(snapshot, provenance))
        return LIFECYCLE_AUDIT_FAILED;

    out_report->snapshot = *snapshot;
    out_report->verify_result = verify_result;
    out_report->provenance = *provenance;
    out_report->assertion = assertion;
    out_report->freeze_version = (uint32_t)LIFECYCLE_FREEZE_VERSION;

    return LIFECYCLE_AUDIT_OK;
}
