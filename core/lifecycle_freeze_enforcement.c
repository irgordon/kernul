/*
 * core/lifecycle_freeze_enforcement.c
 *
 * Compile-time lifecycle freeze enforcement checks.
 */

#include <stddef.h>

#include <kernul/lifecycle_freeze_enforcement.h>
#include <kernul/lifecycle_freeze_baseline.h>

LIFECYCLE_FREEZE_ASSERT(LIFECYCLE_FREEZE_VERSION > 0U,
                        "lifecycle freeze baseline version must be non-zero");

LIFECYCLE_FREEZE_ASSERT(sizeof(lifecycle_snapshot_t) == LIFECYCLE_STATE_SIZE,
                        "lifecycle_state modified after freeze");
LIFECYCLE_FREEZE_ASSERT(offsetof(lifecycle_snapshot_t, finalized)
                            == LIFECYCLE_STATE_FINALIZED_OFFSET,
                        "lifecycle_state.finalized offset changed after freeze");
LIFECYCLE_FREEZE_ASSERT(offsetof(lifecycle_snapshot_t, ready)
                            == LIFECYCLE_STATE_READY_OFFSET,
                        "lifecycle_state.ready offset changed after freeze");
LIFECYCLE_FREEZE_ASSERT(offsetof(lifecycle_snapshot_t, recovery_outcome)
                            == RECOVERY_OUTCOME_OFFSET,
                        "lifecycle_state.recovery_outcome offset changed after freeze");
LIFECYCLE_FREEZE_ASSERT(offsetof(lifecycle_snapshot_t, retry_outcome)
                            == RETRY_OUTCOME_OFFSET,
                        "lifecycle_state.retry_outcome offset changed after freeze");

LIFECYCLE_FREEZE_ASSERT(sizeof(lifecycle_provenance_t) == LIFECYCLE_PROVENANCE_SIZE,
                        "lifecycle_provenance modified after freeze");
LIFECYCLE_FREEZE_ASSERT(
    offsetof(lifecycle_provenance_t, recovery_outcome_timestamp)
        == LIFECYCLE_PROVENANCE_RECOVERY_OUTCOME_TIMESTAMP_OFFSET,
    "lifecycle_provenance.recovery_outcome_timestamp offset changed after freeze");
LIFECYCLE_FREEZE_ASSERT(
    offsetof(lifecycle_provenance_t, retry_outcome_timestamp)
        == LIFECYCLE_PROVENANCE_RETRY_OUTCOME_TIMESTAMP_OFFSET,
    "lifecycle_provenance.retry_outcome_timestamp offset changed after freeze");

LIFECYCLE_FREEZE_ASSERT(
    _Generic(&lifecycle_introspect, lifecycle_introspect_sig_t : 1, default : 0),
    "lifecycle_introspect signature changed after freeze");
LIFECYCLE_FREEZE_ASSERT(
    _Generic(&lifecycle_get_provenance, lifecycle_get_provenance_sig_t : 1, default : 0),
    "lifecycle_get_provenance signature changed after freeze");

const unsigned int lifecycle_freeze_enforcement_link_anchor = LIFECYCLE_FREEZE_VERSION;
