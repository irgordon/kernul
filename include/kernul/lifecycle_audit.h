#ifndef KERNUL_LIFECYCLE_AUDIT_H
#define KERNUL_LIFECYCLE_AUDIT_H

/*
 * include/kernul/lifecycle_audit.h
 *
 * Kernel-internal deterministic lifecycle audit reporting.
 * Phase 37, Task 1 boundary.
 */

#include <kernul/types.h>
#include <kernul/lifecycle_introspection.h>
#include <kernul/lifecycle_verification.h>
#include <kernul/lifecycle_provenance.h>
#include <kernul/lifecycle_assertion.h>

typedef enum lifecycle_audit_result {
    LIFECYCLE_AUDIT_OK = 0,
    LIFECYCLE_AUDIT_FAILED,
} lifecycle_audit_result_t;

typedef struct lifecycle_audit_report {
    /* structured audit data derived from caller inputs */
    lifecycle_snapshot_t snapshot;
    lifecycle_verify_result_t verify_result;
    lifecycle_provenance_t provenance;
    lifecycle_assert_result_t assertion;
    uint32_t freeze_version;
} lifecycle_audit_report_t;

lifecycle_audit_result_t
lifecycle_generate_audit_report(
    const lifecycle_snapshot_t *snapshot,
    lifecycle_verify_result_t verify_result,
    const lifecycle_provenance_t *provenance,
    lifecycle_assert_result_t assertion,
    lifecycle_audit_report_t *out_report);

#endif /* KERNUL_LIFECYCLE_AUDIT_H */
