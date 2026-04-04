#ifndef KERNUL_SESSION_RECOVERY_OUTCOME_OBSERVE_H
#define KERNUL_SESSION_RECOVERY_OUTCOME_OBSERVE_H

/*
 * include/kernul/session_recovery_outcome_observe.h
 *
 * Read-only, readiness-gated recovery outcome snapshot observation surface.
 * Phase 22, Task 1 boundary.
 */

#include <kernul/types.h>
#include <kernul/session_recovery_outcome.h>

struct session;

typedef struct session_recovery_outcome_info {
    session_recovery_outcome_t outcome;
    uint64_t outcome_timestamp;
    uint32_t execution_attempt_count;
} session_recovery_outcome_info_t;

bool
session_get_recovery_outcome_info_acquire(
    const struct session *session,
    session_recovery_outcome_info_t *out_info);

#endif /* KERNUL_SESSION_RECOVERY_OUTCOME_OBSERVE_H */
