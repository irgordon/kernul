#ifndef KERNUL_LIFECYCLE_VERIFICATION_H
#define KERNUL_LIFECYCLE_VERIFICATION_H

/*
 * include/kernul/lifecycle_verification.h
 *
 * Kernel-internal passive lifecycle invariant verification.
 * Phase 32, Task 1 boundary.
 */

#include <kernul/lifecycle_introspection.h>

typedef enum lifecycle_verify_result {
    LIFECYCLE_VERIFY_VERIFIED = 0,
    LIFECYCLE_VERIFY_VIOLATION,
    LIFECYCLE_VERIFY_UNKNOWN,
} lifecycle_verify_result_t;

lifecycle_verify_result_t
lifecycle_verify(const lifecycle_snapshot_t *snapshot);

#endif /* KERNUL_LIFECYCLE_VERIFICATION_H */
