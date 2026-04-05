#ifndef KERNUL_SESSION_RETRY_EXECUTION_H
#define KERNUL_SESSION_RETRY_EXECUTION_H

/*
 * include/kernul/session_retry_execution.h
 *
 * Kernel-internal explicit single-attempt retry execution policy.
 * Phase 26, Task 1 boundary.
 *
 * This header defines one synchronous execution action:
 * perform exactly one retry attempt for a ready session that has already
 * been explicitly authorized for retry.
 *
 * Execution semantics:
 *   - Authorization consumption is the sole execution authority gate.
 *   - Exactly one attempt is performed per successful authorization consume.
 *   - No retry loops, scheduling, coordination, or policy evaluation.
 *   - Execution result is transient in this phase and not lifecycle-stored.
 */

#include <kernul/session_retry_execution_result.h>

struct session;

session_retry_execution_result_t
session_execute_retry(struct session *session);

#endif /* KERNUL_SESSION_RETRY_EXECUTION_H */
