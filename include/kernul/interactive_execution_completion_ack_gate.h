#ifndef KERNUL_INTERACTIVE_EXECUTION_COMPLETION_ACK_GATE_H
#define KERNUL_INTERACTIVE_EXECUTION_COMPLETION_ACK_GATE_H

/*
 * include/kernul/interactive_execution_completion_ack_gate.h
 *
 * Kernel-internal completion acknowledgment gate policy contract.
 * Phase 12, Task 2 boundary.
 *
 * This header defines one externally triggered reaction only:
 * publish a session-owned completion acknowledgment gate when the published
 * execution outcome view kind is COMPLETED.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Scope and dependency rules:
 *   This surface consumes outcome visibility only through
 *   interactive_execution_outcome_view_acquire(struct session *).
 *   It does not read session outcome record fields directly.
 *   It does not consume outcome aggregates.
 *
 * Gate semantics:
 *   interactive_execution_completion_ack_publish_if_completed() is explicit
 *   caller-triggered publication only. No background evaluation, polling,
 *   deferred work, callbacks, or asynchronous behavior is introduced.
 *   Unpublished outcome is a no-op.
 *   FAILED outcome is a no-op by intentional asymmetry.
 *   COMPLETED outcome publishes the session-owned completion ack gate with
 *   release semantics.
 *   Publication is one-shot, monotonic, and idempotent for session lifetime.
 *
 * Acquire semantics:
 *   interactive_execution_completion_ack_acquire() performs explicit acquire
 *   visibility on the completion ack publication marker and returns the stable
 *   session-owned view pointer when published, or NULL when unpublished.
 */

#include <kernul/types.h>

struct session;

struct interactive_execution_completion_ack_view {
    u32 published;
};

void
interactive_execution_completion_ack_publish_if_completed(struct session *session);

const struct interactive_execution_completion_ack_view *
interactive_execution_completion_ack_acquire(struct session *session);

#endif /* KERNUL_INTERACTIVE_EXECUTION_COMPLETION_ACK_GATE_H */
