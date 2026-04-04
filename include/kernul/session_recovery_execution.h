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
 *   - Authorization is consumed before execution begins.
 *   - Exactly one attempt is performed per successful authorization consume.
 *   - Outcome is informational and does not reopen lifecycle transitions.
 *   - No retries, scheduling, coordination, or policy evaluation are defined.
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
