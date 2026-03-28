#ifndef KERNUL_INTERACTIVE_RUNNABLE_H
#define KERNUL_INTERACTIVE_RUNNABLE_H

/*
 * include/kernul/interactive_runnable.h
 *
 * Kernel-internal interactive runnable enqueue contract. Phase 7 boundary.
 *
 * This header defines structural runnable membership registration only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Runnable state constants:
 *   INTERACTIVE_RUNNABLE_STATE_NONE and
 *   INTERACTIVE_RUNNABLE_STATE_ENQUEUED are structural constants in this phase.
 *   Runnable state represents membership only.
 *   Register membership means creating or recording a runnable eligibility
 *   marker discoverable by the scheduler; it does not insert the consumer into
 *   any scheduling queue or alter execution state.
 *   Runnable state represents membership only; runtime scheduling states such
 *   as running, blocked, waiting, or preempted are defined exclusively in
 *   scheduler subsystems.
 *   The runnable state field represents a single membership transition.
 *
 * Runnable record and ownership:
 *   struct interactive_runnable is a kernel-internal runnable membership
 *   record.
 *   It is logically associated with the session of its admission; it does not
 *   own or extend the lifetime of that session.
 *   The lifetime of a runnable record is bounded by the lifetime of its
 *   associated session in this phase.
 *   Destruction and cleanup semantics are not defined in this phase.
 *   The runnable record does not own struct interactive_admission or
 *   struct process_group.
 *   @admission and @consumer_group are borrowed associations only.
 *   No ownership transfer is implied.
 *   No reference counting is introduced.
 *   Creation helpers must not call interactive_admission_get(),
 *   process_group_get(), or any future equivalent.
 *   No automatic cleanup or cascading behavior is defined.
 *
 * Runnable enqueue semantics and scope:
 *   Runnable records mark membership in the scheduler's runnable domain, not
 *   execution.
 *   Enqueued means the consumer is registered as runnable; it does not select,
 *   order, prioritize, or execute the consumer.
 *   Runnable enqueue validates the interactive execution chain:
 *     session -> controlling_terminal -> interactive_console ->
 *     interactive_activation -> interactive_readiness ->
 *     interactive_admission -> interactive_runnable
 *   Runnable enqueue does not select runnable entities, assign priority or
 *   weight, enforce fairness or time slicing, preempt running entities,
 *   deliver signals, enforce job control, or perform terminal I/O.
 *   This surface exists solely to establish runnable membership.
 *
 * Validity, uniqueness, idempotence, and monotonicity invariants:
 *   A session may have zero or one interactive runnable record.
 *   A runnable record may be created only after an interactive admission record
 *   exists.
 *   Duplicate or conflicting runnable enqueues are invalid input.
 *   Creating runnable membership with identical inputs returns the existing
 *   runnable record.
 *   Runnable membership is a monotonic transition in this phase; once
 *   enqueued, the membership state does not revert within this contract.
 *   Runnable removal or dequeue is not defined in this phase.
 *   This contract defines a single runnable membership invariant.
 */

#include <kernul/types.h>

struct interactive_admission;
struct process_group;

#define INTERACTIVE_RUNNABLE_STATE_NONE      0U
#define INTERACTIVE_RUNNABLE_STATE_ENQUEUED  1U

struct interactive_runnable {
    struct interactive_admission *admission;
    struct process_group *consumer_group;
    u32 state;
};

struct interactive_runnable *
interactive_runnable_enqueue(struct interactive_admission *admission);

u32
interactive_runnable_state(const struct interactive_runnable *ir);

#endif /* KERNUL_INTERACTIVE_RUNNABLE_H */
