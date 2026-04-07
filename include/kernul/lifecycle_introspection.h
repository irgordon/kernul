#ifndef KERNUL_LIFECYCLE_INTROSPECTION_H
#define KERNUL_LIFECYCLE_INTROSPECTION_H

/*
 * include/kernul/lifecycle_introspection.h
 *
 * Kernel-internal read-only lifecycle introspection.
 * Phase 31, Task 1 boundary.
 */

#include <kernul/types.h>

struct session;

typedef enum lifecycle_introspect_result {
    LIFECYCLE_INTROSPECT_OK = 0,
    LIFECYCLE_INTROSPECT_NOT_READY,
    LIFECYCLE_INTROSPECT_FAILED,
} lifecycle_introspect_result_t;

typedef struct lifecycle_snapshot {
    /* foundation */
    bool finalized;
    bool ready;

    /* ownership */
    uint64_t owner_id;
    bool reclaimed;

    /* recovery */
    bool recovery_eligible;
    bool recovery_authorized;
    bool recovery_executed;
    int  recovery_execution_result;
    int  recovery_outcome;

    /* retry */
    int  retry_policy;
    bool retry_authorized;
    bool retry_executed;
    int  retry_execution_result;
    int  retry_outcome;
} lifecycle_snapshot_t;

lifecycle_introspect_result_t
lifecycle_introspect(
    const struct session *session,
    lifecycle_snapshot_t *out_snapshot);

#endif /* KERNUL_LIFECYCLE_INTROSPECTION_H */
