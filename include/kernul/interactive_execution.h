#ifndef KERNUL_INTERACTIVE_EXECUTION_H
#define KERNUL_INTERACTIVE_EXECUTION_H

/*
 * include/kernul/interactive_execution.h
 *
 * Kernel-internal interactive execution transfer contract.
 * Phase 8 boundary.
 *
 * This header defines structural execution transfer only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Required dependencies:
 *   This contract depends on struct interactive_switch_operands and
 *   arch_cpu_state_switch().
 *   No new architecture-specific execution mechanism is introduced here.
 *
 * Execution state constants:
 *   INTERACTIVE_EXECUTION_STATE_NONE and
 *   INTERACTIVE_EXECUTION_STATE_EXECUTED are structural constants.
 *   State represents actual execution transfer only.
 *   Executed means arch_cpu_state_switch() has been invoked.
 *   State does not encode scheduling policy, fairness, or runtime management.
 *
 * Execution record and ownership:
 *   struct interactive_execution is a kernel-internal execution transfer
 *   record.
 *   It is logically associated with the session of its switch operands.
 *   It does not own or extend the lifetime of the session, operands,
 *   or process group.
 *   @operands and @consumer_group are borrowed associations only.
 *   No ownership transfer is implied.
 *   No reference counting is introduced.
 *   Destruction and cleanup semantics are not defined in this phase.
 *   No automatic cleanup or cascading behavior is defined.
 *
 * Execution transfer semantics and scope:
 *   Execution records mark real execution transfer.
 *   Execution transfer means invoking arch_cpu_state_switch(from, to)
 *   exactly once per successful execution transfer in real execution builds.
 *   Transfer validates the interactive chain:
 *     session -> controlling_terminal -> interactive_console ->
 *     interactive_activation -> interactive_readiness ->
 *     interactive_admission -> interactive_runnable ->
 *     interactive_dispatch -> interactive_execution_target ->
 *     interactive_switch_operands -> interactive_execution ->
 *     arch_cpu_state_switch()
 *   This surface exists solely to cross the execution boundary.
 *   It does not select runnable entities, assign priority or weight,
 *   enforce fairness/time slicing, implement preemption policy,
 *   manage run queues, deliver signals, enforce job control,
 *   or perform terminal I/O.
 *
 * Control-flow semantics:
 *   In real execution builds, interactive_execution_transfer() does not return
 *   in the original execution context.
 *   Control resumes only when a later architecture switch restores this
 *   context through mechanisms outside this contract.
 *   No return-value semantics are defined beyond successful invocation.
 *
 * Execution gating and verification safety:
 *   Transfer must be explicitly guarded so verification/test builds do not
 *   perform a real context switch.
 *   In test/verification configurations, transfer simulates success without
 *   invoking arch_cpu_state_switch().
 *   Simulation preserves deterministic behavior, idempotence rules,
 *   and monotonic state transitions.
 *   Real arch_cpu_state_switch() invocation is permitted only in non-test
 *   execution builds.
 *
 * Validity, uniqueness, idempotence, and monotonicity invariants:
 *   A session may have zero or one interactive execution record.
 *   An execution record may be created only after switch operands exist.
 *   Duplicate or conflicting execution attempts are invalid input.
 *   Identical inputs return the existing execution record.
 *   In this phase, identical inputs means the same
 *   switch-operands-associated session and consumer group.
 *   Execution is a monotonic transition; once executed, state does not revert.
 *   Execution repetition or rollback is not defined in this phase.
 *   This contract defines a single execution-transfer invariant.
 */

#include <kernul/interactive_switch_operands.h>
#include <kernul/types.h>

struct process_group;

#define INTERACTIVE_EXECUTION_STATE_NONE      0U
#define INTERACTIVE_EXECUTION_STATE_EXECUTED  1U

struct interactive_execution {
    struct interactive_switch_operands *operands;
    struct process_group *consumer_group;
    u32 state;
};

struct interactive_execution *
interactive_execution_transfer(struct interactive_switch_operands *operands);

#endif /* KERNUL_INTERACTIVE_EXECUTION_H */
