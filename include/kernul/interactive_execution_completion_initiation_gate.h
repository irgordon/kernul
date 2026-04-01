#ifndef KERNUL_INTERACTIVE_EXECUTION_COMPLETION_INITIATION_GATE_H
#define KERNUL_INTERACTIVE_EXECUTION_COMPLETION_INITIATION_GATE_H

/*
 * include/kernul/interactive_execution_completion_initiation_gate.h
 *
 * Kernel-internal completion acknowledgment-driven initiation gate policy.
 * Phase 13, Task 2 boundary.
 *
 * This header defines one control policy:
 * completion acknowledgment publication can irreversibly forbid future
 * execution initiation for the owning session.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Scope and dependency rules:
 *   This surface consumes only
 *   interactive_execution_completion_ack_acquire(struct session *).
 *   It does not consume outcome record/view/aggregate surfaces.
 *   It does not consume failure acknowledgment surfaces.
 *
 * Enforcement boundary:
 *   interactive_execution_initiation_is_permitted_acquire() is consumed only
 *   at the execution initiation boundary.
 *   No selection/scheduling/container/outcome surface may enforce this policy.
 *
 * Visibility and monotonicity:
 *   interactive_execution_initiation_forbid_on_completion_ack() acquires the
 *   completion acknowledgment gate and, when published, release-publishes a
 *   session-owned initiation-permitted marker as forbidden.
 *   The transition is one-way, monotonic, idempotent, and session-lifetime
 *   bounded with no reset path.
 */

#include <kernul/types.h>

struct session;

void
interactive_execution_initiation_forbid_on_completion_ack(struct session *session);

#endif /* KERNUL_INTERACTIVE_EXECUTION_COMPLETION_INITIATION_GATE_H */
