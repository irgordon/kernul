#ifndef KERNUL_INTERACTIVE_EXECUTION_FAILURE_INITIATION_GATE_H
#define KERNUL_INTERACTIVE_EXECUTION_FAILURE_INITIATION_GATE_H

/*
 * include/kernul/interactive_execution_failure_initiation_gate.h
 *
 * Kernel-internal failure acknowledgment-driven initiation gate policy.
 * Phase 13, Task 1 boundary.
 *
 * This header defines one control policy:
 * failure acknowledgment publication can irreversibly forbid future execution
 * initiation for the owning session.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Scope and dependency rules:
 *   This surface consumes only
 *   interactive_execution_failure_ack_acquire(struct session *).
 *   It does not consume outcome record/view/aggregate surfaces.
 *   It does not consume completion acknowledgment surfaces.
 *
 * Enforcement boundary:
 *   interactive_execution_initiation_is_permitted_acquire() is consumed only
 *   at the execution initiation boundary.
 *   No selection/scheduling/container/outcome surface may enforce this policy.
 *
 * Visibility and monotonicity:
 *   interactive_execution_initiation_forbid_on_failure_ack() acquires the
 *   failure acknowledgment gate and, when published, release-publishes a
 *   session-owned initiation-permitted marker as forbidden.
 *   The transition is one-way, monotonic, idempotent, and session-lifetime
 *   bounded with no reset path.
 */

#include <kernul/types.h>

struct session;

void
interactive_execution_initiation_forbid_on_failure_ack(struct session *session);

bool
interactive_execution_initiation_is_permitted_acquire(const struct session *session);

#endif /* KERNUL_INTERACTIVE_EXECUTION_FAILURE_INITIATION_GATE_H */
