#ifndef KERNUL_INTERACTIVE_EXECUTION_TARGET_H
#define KERNUL_INTERACTIVE_EXECUTION_TARGET_H

/*
 * include/kernul/interactive_execution_target.h
 *
 * Kernel-internal interactive execution target preparation contract.
 * Phase 7 boundary.
 *
 * This header defines structural execution-target preparation only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Execution target state constants:
 *   INTERACTIVE_EXECUTION_TARGET_STATE_NONE and
 *   INTERACTIVE_EXECUTION_TARGET_STATE_PREPARED are structural constants in
 *   this phase.
 *   Execution target state represents preparation only.
 *   Prepared means dispatch selection has been resolved into a preparation
 *   marker; it does not bind to a CPU, reserve resources, or initiate
 *   execution state changes.
 *   The execution target record does not identify a CPU, run queue, scheduling
 *   resource, or execution token; those associations are defined exclusively in
 *   scheduler execution subsystems.
 *   Preparation does not identify or bind a specific thread in this phase;
 *   thread-level execution selection is defined in later scheduler execution
 *   tasks.
 *   Runtime execution states are defined exclusively in scheduler and
 *   architecture execution subsystems.
 *
 * Execution target record and ownership:
 *   struct interactive_execution_target is a kernel-internal preparation
 *   record.
 *   It is logically associated with the session of its dispatch; it does not
 *   own or extend the lifetime of that session.
 *   The lifetime of an execution target record is bounded by the lifetime of
 *   its associated session in this phase.
 *   Destruction and cleanup semantics are not defined in this phase.
 *   The execution target record does not own struct interactive_dispatch or
 *   struct process_group.
 *   @dispatch and @consumer_group are borrowed associations only.
 *   No ownership transfer is implied.
 *   No reference counting is introduced.
 *   Creation helpers must not call interactive_dispatch_get(),
 *   process_group_get(), or any future equivalent.
 *   No automatic cleanup or cascading behavior is defined.
 *
 * Preparation semantics and scope:
 *   Preparation records mark execution target resolution, not execution.
 *   Preparation resolves dispatch selection into a preparation record suitable
 *   for later architecture-specific context switching.
 *   Preparation validates the interactive chain:
 *     session -> controlling_terminal -> interactive_console ->
 *     interactive_activation -> interactive_readiness ->
 *     interactive_admission -> interactive_runnable ->
 *     interactive_dispatch -> interactive_execution_target
 *   Preparation does not perform a context switch, execute threads, bind to a
 *   CPU, identify or bind a specific thread, reserve timeslices or resources,
 *   assign priority or weight, enforce fairness or time slicing, implement
 *   preemption policy, manage run queues, deliver signals, enforce job control,
 *   or perform terminal I/O.
 *
 * Validity, uniqueness, idempotence, and monotonicity invariants:
 *   A session may have zero or one interactive execution target record.
 *   An execution target record may be created only after an interactive
 *   dispatch record exists.
 *   Duplicate or conflicting preparations are invalid input.
 *   Creating preparation with identical inputs returns the existing preparation
 *   record.
 *   In this phase, identical inputs means the same dispatch-associated session
 *   and consumer group.
 *   Preparation is a monotonic transition in this phase; once prepared, the
 *   state does not revert within this contract.
 *   Preparation replacement or withdrawal is not defined in this phase.
 *   This contract defines a single preparation invariant.
 */

#include <kernul/types.h>

struct interactive_dispatch;
struct process_group;

#define INTERACTIVE_EXECUTION_TARGET_STATE_NONE      0U
#define INTERACTIVE_EXECUTION_TARGET_STATE_PREPARED  1U

struct interactive_execution_target {
    struct interactive_dispatch *dispatch;
    struct process_group *consumer_group;
    u32 state;
};

struct interactive_execution_target *
interactive_execution_prepare(struct interactive_dispatch *dispatch);

u32
interactive_execution_target_state(const struct interactive_execution_target *iet);

#endif /* KERNUL_INTERACTIVE_EXECUTION_TARGET_H */
