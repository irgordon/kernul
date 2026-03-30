#ifndef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#define KERNUL_INTERACTIVE_SCHEDULER_STATE_H

/*
 * include/kernul/interactive_scheduler_state.h
 *
 * Kernel-internal interactive scheduler state materialization contract.
 * Phase 9 boundary.
 *
 * This header defines persistent scheduler structure materialization only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Required dependencies:
 *   This contract depends on struct interactive_runnable and struct session.
 *   Scheduler state exists beneath selection and above runnable membership.
 *
 * Scheduler state constants:
 *   INTERACTIVE_SCHEDULER_STATE_NONE and
 *   INTERACTIVE_SCHEDULER_STATE_READY are structural constants.
 *   Ready means runnable membership has been materialized into scheduler
 *   state.
 *   State encodes persistent scheduler structure only, not scheduler behavior.
 *
 * Scheduler state record and ownership:
 *   struct interactive_scheduler_state is a kernel-internal scheduler-state
 *   record.
 *   Scheduler state is owned per session in this phase.
 *   The scheduler-state record does not own session or runnable membership.
 *   @session and @runnable are borrowed associations only.
 *   No ownership transfer is implied.
 *   No reference counting is introduced.
 *   Scheduler state is created only after runnable membership exists.
 *   Scheduler state remains valid for the lifetime of the session unless
 *   explicitly replaced by a later phase.
 *   No global or subsystem-wide scheduler-state object is permitted in this
 *   phase.
 *
 * Ordering invariant:
 *   Ordering is an invariant separate from container implementation.
 *   Authoritative ordering source is interactive runnable membership ordering.
 *   Container choice must not define scheduler policy.
 *   FIFO/LIFO/priority semantics are not implied by container behavior here.
 *   This surface may store ordering representation but must not define
 *   ordering policy.
 *
 * Scheduler-state semantics and scope:
 *   This surface materializes runnable membership into persistent scheduler
 *   state.
 *   It does not select among candidates.
 *   It does not mutate runnable membership.
 *   It does not affect readiness or execution.
 *   It does not implement fairness, time slicing, or preemption.
 *   It does not manage run queues, bind CPUs, reserve resources, deliver
 *   signals, enforce job control, or perform terminal I/O.
 *
 * Determinism, uniqueness, idempotence, monotonicity, and conflict:
 *   For a given session pointer and runnable pointer identity, creation is
 *   deterministic and idempotent.
 *   No iteration-based ordering decisions may occur in creation.
 *   No traversal logic may define or infer ordering.
 *   A session may have zero or one scheduler-state record in this phase.
 *   The bounded single-slot model is a phase-local per-session stand-in.
 *   Identical inputs are defined by pointer identity of:
 *     struct session *
 *     struct interactive_runnable *
 *   Identical inputs return the existing scheduler-state record.
 *   Conflict: if a session already has scheduler state associated with a
 *   different runnable pointer, creation returns NULL.
 *   Once state transitions to READY, it does not revert in this phase.
 *   Subsequent identical-input calls observe the same READY state with no
 *   rollback.
 *
 * Dependency prohibitions:
 *   Scheduler-state code must not invoke selection, execution transfer, or
 *   architecture switching.
 *   Scheduler-state code must not include headers whose primary purpose is
 *   execution transfer, architecture switching, or timekeeping.
 *   This prohibition applies to direct and indirect inclusion paths.
 */

#include <kernul/types.h>
#include <kernul/session.h>

struct interactive_runnable;

#define INTERACTIVE_SCHEDULER_STATE_NONE   0U
#define INTERACTIVE_SCHEDULER_STATE_READY  1U

struct interactive_scheduler_state *
interactive_scheduler_state_create(
    struct session *session,
    struct interactive_runnable *runnable
);

u32
interactive_scheduler_state_state(
    const struct interactive_scheduler_state *state
);

#endif /* KERNUL_INTERACTIVE_SCHEDULER_STATE_H */
