#ifndef KERNUL_SESSION_RECOVERY_EXECUTION_H
#define KERNUL_SESSION_RECOVERY_EXECUTION_H

/*
 * include/kernul/session_recovery_execution.h
 *
 * Kernel-internal explicit single-attempt recovery execution policy.
 * Phase 20, Task 1 boundary.
 *
 * This header defines one synchronous execution action:
 * perform exactly one recovery attempt for a finalized session that has
 * already been explicitly authorized.
 *
 * Execution semantics:
 *   - Authorization consumption is the sole execution authority gate.
 *   - Exactly one attempt is performed per successful authorization consume.
 *   - Outcome is informational and does not reopen lifecycle transitions.
 *   - No retries, scheduling, coordination, or policy evaluation are defined.
 *   - If no primitive exists in this phase, the attempted execution
 *     deterministically fails.
 *
 * Observation rule:
 *   Storage-observed execution completion/result queries are permissive for
 *   NULL or not-ready input and return safe defaults.
 *   NULL/not-ready observations are non-authoritative lifecycle facts.
 */

#include <kernul/types.h>

struct session;

typedef enum session_recovery_execution_result {
    SESSION_RECOVERY_EXEC_OK = 0,
    SESSION_RECOVERY_EXEC_NOT_FINALIZED,
    SESSION_RECOVERY_EXEC_NOT_AUTHORIZED,
    SESSION_RECOVERY_EXEC_FAILED,
} session_recovery_execution_result_t;

session_recovery_execution_result_t
session_execute_recovery(struct session *session);

#endif /* KERNUL_SESSION_RECOVERY_EXECUTION_H */
