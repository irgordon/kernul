#ifndef KERNUL_INTERACTIVE_EXECUTION_OUTCOME_RECORD_H
#define KERNUL_INTERACTIVE_EXECUTION_OUTCOME_RECORD_H

/*
 * include/kernul/interactive_execution_outcome_record.h
 *
 * Kernel-internal interactive execution outcome publication contract.
 * Phase 11, Task 1 boundary.
 *
 * This header defines passive outcome record publication and acquire only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Required dependencies:
 *   This contract depends on struct session and transfer boundary return
 *   record type struct interactive_execution.
 *
 * Outcome record and ownership:
 *   struct interactive_execution_outcome_record is session-owned storage.
 *   The record does not own transfer boundary records.
 *   No allocation is introduced.
 *
 * Publication and scope:
 *   interactive_execution_outcome_record_publish_from_transfer() maps
 *   transfer boundary return state to a single outcome kind and publishes
 *   visibility with explicit release semantics.
 *   Mapping is total and deterministic:
 *     - executed transfer state maps to COMPLETED
 *     - all other states map to FAILED
 *   Re-publication is an explicit deterministic replacement rule:
 *   a new publish call replaces the previous session-owned kind.
 *   No retry, backoff, timing, fairness, priority, preemption, CPU binding,
 *   or upstream reaction behavior is defined here.
 *
 * Acquire:
 *   interactive_execution_outcome_record_acquire() consumes publication with
 *   explicit acquire semantics and returns the published session-owned record,
 *   or NULL when unpublished.
 */

struct session;
struct interactive_execution;

enum interactive_execution_outcome_kind {
    INTERACTIVE_EXECUTION_OUTCOME_COMPLETED,
    INTERACTIVE_EXECUTION_OUTCOME_FAILED,
};

struct interactive_execution_outcome_record {
    enum interactive_execution_outcome_kind kind;
};

struct interactive_execution_outcome_record *
interactive_execution_outcome_record_publish_from_transfer(
    struct session *session,
    struct interactive_execution *transfer_result
);

const struct interactive_execution_outcome_record *
interactive_execution_outcome_record_acquire(
    struct session *session
);

#endif /* KERNUL_INTERACTIVE_EXECUTION_OUTCOME_RECORD_H */
