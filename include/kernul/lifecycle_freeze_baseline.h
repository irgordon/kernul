#ifndef KERNUL_LIFECYCLE_FREEZE_BASELINE_H
#define KERNUL_LIFECYCLE_FREEZE_BASELINE_H

/*
 * include/kernul/lifecycle_freeze_baseline.h
 *
 * Canonical lifecycle freeze metadata baseline.
 * Structural changes require baseline update and version increment.
 */

#include <kernul/lifecycle_introspection.h>
#include <kernul/lifecycle_provenance.h>

#define LIFECYCLE_FREEZE_VERSION 1U

/* Frozen lifecycle snapshot layout baseline. */
#define LIFECYCLE_STATE_SIZE 48U
#define LIFECYCLE_STATE_FINALIZED_OFFSET 0U
#define LIFECYCLE_STATE_READY_OFFSET 1U
#define RECOVERY_OUTCOME_OFFSET 24U
#define RETRY_OUTCOME_OFFSET 40U

/* Frozen lifecycle provenance layout baseline. */
#define LIFECYCLE_PROVENANCE_SIZE 96U
#define LIFECYCLE_PROVENANCE_RECOVERY_OUTCOME_TIMESTAMP_OFFSET 56U
#define LIFECYCLE_PROVENANCE_RETRY_OUTCOME_TIMESTAMP_OFFSET 88U

/* Frozen lifecycle function signature baselines. */
typedef lifecycle_introspect_result_t (*lifecycle_introspect_sig_t)(
    const struct session *session,
    lifecycle_snapshot_t *out_snapshot);

typedef int (*lifecycle_get_provenance_sig_t)(
    const struct session *session,
    lifecycle_provenance_t *out_provenance);

#endif /* KERNUL_LIFECYCLE_FREEZE_BASELINE_H */
