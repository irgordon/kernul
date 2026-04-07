#ifndef KERNUL_LIFECYCLE_PROVENANCE_H
#define KERNUL_LIFECYCLE_PROVENANCE_H

/*
 * include/kernul/lifecycle_provenance.h
 *
 * Kernel-internal read-only lifecycle provenance.
 * Phase 33, Task 1 boundary.
 */

#include <kernul/lifecycle_introspection.h>

typedef enum lifecycle_provenance_result {
    LIFECYCLE_PROVENANCE_OK = 0,
    LIFECYCLE_PROVENANCE_NOT_READY,
    LIFECYCLE_PROVENANCE_FAILED,
} lifecycle_provenance_result_t;

typedef struct lifecycle_provenance {
    /* foundation */
    uint64_t finalization_timestamp;
    uint64_t readiness_timestamp;

    /* ownership */
    uint64_t ownership_timestamp;
    uint64_t reclamation_timestamp;

    /* recovery */
    uint64_t recovery_eligibility_timestamp;
    uint64_t recovery_authorization_timestamp;
    uint64_t recovery_execution_timestamp;
    uint64_t recovery_outcome_timestamp;

    /* retry */
    uint64_t retry_policy_timestamp;
    uint64_t retry_authorization_timestamp;
    uint64_t retry_execution_timestamp;
    uint64_t retry_outcome_timestamp;
} lifecycle_provenance_t;

/*
 * Provenance contract:
 * - Derived from existing lifecycle facts only (no parallel storage).
 * - Observation is readiness-gated and read-only.
 * - All underlying lifecycle reads are acquire-ordered.
 *
 * Timestamp semantics:
 * - 0 means the corresponding transition is absent/not observed.
 * - Monotonicity is guaranteed only within a lifecycle dimension.
 * - Cross-dimension ordering is not guaranteed.
 */
int
lifecycle_get_provenance(
    const struct session *session,
    lifecycle_provenance_t *out_provenance);

#endif /* KERNUL_LIFECYCLE_PROVENANCE_H */
