#ifndef KERNUL_INTERACTIVE_SELECTION_H
#define KERNUL_INTERACTIVE_SELECTION_H

/*
 * include/kernul/interactive_selection.h
 *
 * Kernel-internal interactive scheduler selection policy contract.
 * Phase 8 boundary.
 *
 * This header defines scheduler policy selection only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Required dependencies:
 *   This contract depends on struct interactive_runnable,
 *   struct interactive_scheduler_state,
 *   struct interactive_scheduler_container,
 *   struct interactive_dispatch, and struct interactive_execution_target.
 *   Selection reads candidate identity from a per-session
 *   interactive_scheduler_container membership view.
 *   Selection is strictly upstream of execution-target preparation and
 *   execution transfer.
 *
 * Selection state constants:
 *   INTERACTIVE_SELECTION_STATE_NONE and
 *   INTERACTIVE_SELECTION_STATE_SELECTED are structural constants.
 *   Selected means a runnable candidate has been chosen for dispatch.
 *   State encodes policy decision only.
 *   State does not encode execution, CPU binding, or resource allocation.
 *
 * Selection record and ownership:
 *   struct interactive_selection is a kernel-internal selection record.
 *   It is logically associated with the session of its runnable input.
 *   It does not own or extend the lifetime of that session.
 *   The lifetime of a selection record is bounded by the lifetime of its
 *   associated session in this phase.
 *   Destruction and cleanup semantics are not defined in this phase.
 *   The selection record does not own struct interactive_runnable,
 *   struct interactive_dispatch, or struct process_group.
 *   @runnable, @dispatch, and @consumer_group are borrowed associations only.
 *   No ownership transfer is implied.
 *   No reference counting is introduced.
 *   No automatic cleanup or cascading behavior is defined.
 *
 * Selection semantics and scope:
 *   This contract defines the first scheduler selection policy: choose one
 *   runnable candidate for dispatch.
 *   Selection operates at the interactive runnable / consumer level only.
 *   This contract does not infer or select a specific thread.
 *   This surface may record or associate a dispatch record as part of
 *   selection.
 *   Selection does not implement fairness, time slicing, or preemption.
 *   Selection does not manage run queues, bind CPUs, or reserve resources.
 *   Selection does not deliver signals, enforce job control, or perform
 *   terminal I/O.
 *
 * Deterministic policy rule:
 *   Deterministic ordering is authoritative only from interactive runnable
 *   membership ordering.
 *   Deterministic selection order is defined by interactive runnable
 *   membership ordering, not by incidental traversal behavior.
 *   In this phase, this surface records a runnable candidate read from the
 *   per-session scheduler container membership view under that ordering
 *   authority and does not implement a membership walk.
 *   Dispatch creation order, execution-target preparation order, container
 *   traversal behavior, and incidental pointer ordering are non-authoritative.
 *   This is a phase-local policy boundary only; it is not a long-term fairness
 *   or performance commitment.
 *
 * Validity, uniqueness, idempotence, monotonicity, and visibility:
 *   A session may have zero or one interactive selection record.
 *   The single-slot selection record is scoped per session and exists only as
 *   a deterministic stand-in for the one-per-session invariant.
 *   Selection may occur only after runnable membership exists.
 *   Duplicate or conflicting selections are invalid input.
 *   Identical inputs return the existing selection record.
 *   Idempotence is defined by pointer identity of the runnable input; structural
 *   equivalence is not considered.
 *   Identical inputs additionally require pointer-identity match of the
 *   associated session/consumer-group chain.
 *   Selection is monotonic: once selected, this state does not revert.
 *   Subsequent identical-input calls must observe the same selected record with
 *   no intermediate rollback or transient state.
 *   Selection replacement or withdrawal is not defined in this phase.
 *
 * Dependency prohibitions:
 *   Selection must not invoke execution transfer or architecture switching,
 *   directly or indirectly.
 *   This includes helper/wrapper paths that call:
 *     interactive_execution_transfer()
 *     arch_cpu_state_switch()
 *   This surface must not include execution-transfer headers.
 */

#include <kernul/types.h>

struct interactive_runnable;
struct interactive_dispatch;
struct interactive_execution_target;
struct process_group;

#define INTERACTIVE_SELECTION_STATE_NONE      0U
#define INTERACTIVE_SELECTION_STATE_SELECTED  1U

struct interactive_selection {
    struct interactive_runnable *runnable;
    struct interactive_dispatch *dispatch;
    struct process_group *consumer_group;
    u32 state;
};

struct interactive_selection *
interactive_selection_select(struct interactive_runnable *runnable);

u32
interactive_selection_state(const struct interactive_selection *sel);

#endif /* KERNUL_INTERACTIVE_SELECTION_H */
