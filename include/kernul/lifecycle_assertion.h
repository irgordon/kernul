#ifndef KERNUL_LIFECYCLE_ASSERTION_H
#define KERNUL_LIFECYCLE_ASSERTION_H

/*
 * include/kernul/lifecycle_assertion.h
 *
 * Kernel-internal lifecycle boundary assertion.
 * Phase 34, Task 1 boundary.
 */

#include <kernul/lifecycle_introspection.h>
#include <kernul/lifecycle_verification.h>
#include <kernul/lifecycle_provenance.h>

typedef enum lifecycle_assert_result {
    LIFECYCLE_ASSERT_SATISFIED = 0,
    LIFECYCLE_ASSERT_VIOLATED,
    LIFECYCLE_ASSERT_UNKNOWN,
} lifecycle_assert_result_t;

typedef struct lifecycle_boundary {
    /* declarative boundary expectations */
    bool require_finalized;
    bool require_ready;
    bool require_verified;
    bool require_recovery_complete;
    bool require_retry_complete;
} lifecycle_boundary_t;

lifecycle_assert_result_t
lifecycle_assert(
    const lifecycle_snapshot_t *snapshot,
    lifecycle_verify_result_t verify_result,
    const lifecycle_provenance_t *provenance,
    const lifecycle_boundary_t *boundary);

#endif /* KERNUL_LIFECYCLE_ASSERTION_H */
