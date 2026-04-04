#ifndef KERNUL_SESSION_RECOVERY_OUTCOME_H
#define KERNUL_SESSION_RECOVERY_OUTCOME_H

/*
 * include/kernul/session_recovery_outcome.h
 *
 * Kernel-internal immutable recovery execution outcome recording policy.
 * Phase 21, Task 1 boundary.
 *
 * This header defines declarative single-assignment recording of a completed
 * recovery execution attempt outcome.
 *
 * Recording semantics:
 *   - outcome is lifecycle fact only
 *   - recording is single-assignment and irreversible
 *   - recording depends on observed recovery execution completion
 *   - recording does not trigger retries, scheduling, or coordination
 */

#include <kernul/types.h>

struct session;

typedef enum session_recovery_outcome {
    SESSION_RECOVERY_NOT_ATTEMPTED = 0,
    SESSION_RECOVERY_SUCCEEDED,
    SESSION_RECOVERY_FAILED,
} session_recovery_outcome_t;

typedef enum session_recovery_outcome_result {
    SESSION_RECOVERY_OUTCOME_OK = 0,
    SESSION_RECOVERY_OUTCOME_NOT_READY,
    SESSION_RECOVERY_OUTCOME_ALREADY_RECORDED,
    SESSION_RECOVERY_OUTCOME_FAILED,
} session_recovery_outcome_result_t;

session_recovery_outcome_result_t
session_record_recovery_outcome(
    struct session *session,
    session_recovery_outcome_t outcome);

session_recovery_outcome_t
session_get_recovery_outcome_acquire(const struct session *session);

#endif /* KERNUL_SESSION_RECOVERY_OUTCOME_H */
