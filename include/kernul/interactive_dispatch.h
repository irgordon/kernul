#ifndef KERNUL_INTERACTIVE_DISPATCH_H
#define KERNUL_INTERACTIVE_DISPATCH_H

/*
 * include/kernul/interactive_dispatch.h
 *
 * Kernel-internal interactive scheduler dispatch contract. Phase 7 boundary.
 *
 * This header defines structural dispatch-selection visibility only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Dispatch state constants:
 *   INTERACTIVE_DISPATCH_STATE_NONE and
 *   INTERACTIVE_DISPATCH_STATE_DISPATCHED are structural constants in this
 *   phase.
 *   Dispatch state represents selection acknowledgment only.
 *   Dispatch selection means acknowledgment that the runnable consumer has
 *   been chosen for potential execution; it does not bind the consumer to a
 *   CPU, reserve execution resources, or initiate execution state changes.
 *   The dispatch record does not identify a CPU, thread, or scheduling
 *   resource; those associations are defined exclusively in scheduler
 *   execution subsystems.
 *   Dispatch state does not encode execution, priority, fairness, or time
 *   slicing.
 *   Runtime execution and scheduling states are defined exclusively in
 *   scheduler subsystems.
 *
 * Dispatch record and ownership:
 *   struct interactive_dispatch is a kernel-internal dispatch record.
 *   It is logically associated with the session of its runnable membership; it
 *   does not own or extend the lifetime of that session.
 *   The lifetime of a dispatch record is bounded by the lifetime of its
 *   associated session in this phase.
 *   Destruction and cleanup semantics are not defined in this phase.
 *   The dispatch record does not own struct interactive_runnable or
 *   struct process_group.
 *   @runnable and @consumer_group are borrowed associations only.
 *   No ownership transfer is implied.
 *   No reference counting is introduced.
 *   Creation helpers must not call interactive_runnable_get(),
 *   process_group_get(), or any future equivalent.
 *   No automatic cleanup or cascading behavior is defined.
 *
 * Dispatch semantics and scope:
 *   Dispatch records mark scheduler dispatch selection visibility, not
 *   execution.
 *   Dispatched means the runnable consumer has been selected for dispatch
 *   visibility; it does not execute the consumer, perform a context switch,
 *   bind to a CPU, reserve a timeslice, or alter execution state.
 *   Dispatch validates the interactive execution chain:
 *     session -> controlling_terminal -> interactive_console ->
 *     interactive_activation -> interactive_readiness ->
 *     interactive_admission -> interactive_runnable ->
 *     interactive_dispatch
 *   Dispatch does not perform a context switch, execute threads, bind to a
 *   CPU, reserve execution resources or timeslices, assign priority or
 *   weight, enforce fairness or time slicing, preempt running entities,
 *   deliver signals, enforce job control, or perform terminal I/O.
 *   This surface exists solely to record dispatch selection visibility.
 *
 * Validity, uniqueness, idempotence, and monotonicity invariants:
 *   A session may have zero or one interactive dispatch record.
 *   A dispatch record may be created only after an interactive runnable record
 *   exists.
 *   Duplicate or conflicting dispatch attempts are invalid input.
 *   Creating dispatch with identical inputs returns the existing dispatch
 *   record.
 *   Dispatch is a monotonic transition in this phase; once dispatched, the
 *   state does not revert within this contract.
 *   Dispatch is the terminal monotonic transition in this phase sequence; once
 *   dispatched, the state remains stable until superseded by scheduler
 *   execution mechanics defined in later phases.
 *   Dispatch withdrawal, re-dispatch, or re-selection semantics are not
 *   defined in this phase.
 *   This contract defines a single dispatch handoff invariant.
 */

#include <kernul/types.h>

struct interactive_runnable;
struct process_group;

#define INTERACTIVE_DISPATCH_STATE_NONE       0U
#define INTERACTIVE_DISPATCH_STATE_DISPATCHED 1U

struct interactive_dispatch {
    struct interactive_runnable *runnable;
    struct process_group *consumer_group;
    u32 state;
};

struct interactive_dispatch *
interactive_dispatch_select(struct interactive_runnable *runnable);

u32
interactive_dispatch_state(const struct interactive_dispatch *id);

#endif /* KERNUL_INTERACTIVE_DISPATCH_H */
