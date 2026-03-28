#ifndef KERNUL_INTERACTIVE_SWITCH_OPERANDS_H
#define KERNUL_INTERACTIVE_SWITCH_OPERANDS_H

/*
 * include/kernul/interactive_switch_operands.h
 *
 * Kernel-internal interactive switch operand preparation contract.
 * Phase 7 boundary.
 *
 * This header defines structural switch-operand preparation only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Required dependencies:
 *   This contract depends on struct interactive_execution_target and
 *   arch_cpu_state_t.
 *
 * Switch-operand preparation state constants:
 *   INTERACTIVE_SWITCH_OPERANDS_STATE_NONE and
 *   INTERACTIVE_SWITCH_OPERANDS_STATE_PREPARED are structural constants in
 *   this phase.
 *   State represents operand preparation only.
 *   Prepared means concrete switch operands exist; it does not perform a
 *   context switch, bind a CPU, reserve resources, or initiate execution.
 *   Scheduler policy and runtime execution management are out of scope.
 *
 * Switch-operand record and ownership:
 *   struct interactive_switch_operands is a kernel-internal preparation
 *   record.
 *   It is logically associated with the session of its execution target.
 *   It does not own or extend the lifetime of the session, target,
 *   process_group, or CPU-state operands.
 *   @target, @consumer_group, @from, and @to are borrowed associations only.
 *   No ownership transfer is implied.
 *   No reference counting is introduced.
 *   Destruction and cleanup semantics are not defined in this phase.
 *   No automatic cleanup or cascading behavior is defined.
 *
 * Operand-preparation semantics and scope:
 *   Operand preparation resolves an execution target into concrete
 *   architecture-level switch operands suitable for a later
 *   arch_cpu_state_switch(from, to)-style call.
 *   Operand preparation validates the interactive chain:
 *     session -> controlling_terminal -> interactive_console ->
 *     interactive_activation -> interactive_readiness ->
 *     interactive_admission -> interactive_runnable ->
 *     interactive_dispatch -> interactive_execution_target ->
 *     interactive_switch_operands
 *   Operand preparation does not call arch_cpu_state_switch().
 *   Operand preparation does not perform a context switch, execute threads,
 *   bind to a CPU, reserve timeslices/resources, select among runnable
 *   entities, assign priority/weight, enforce fairness/time slicing,
 *   implement preemption policy, manage run queues, deliver signals, enforce
 *   job control, or perform terminal I/O.
 *
 * Operand source constraints:
 *   @from and @to must be concrete arch_cpu_state_t operands.
 *   In this phase, stub operands must be sourced from statically defined,
 *   non-owning architecture-state placeholders used only for contract
 *   verification.
 *   This task does not define or infer process-group-to-thread selection
 *   policy; it records only the prepared operands required for a later
 *   execution transfer once such selection is defined elsewhere.
 *   The mechanism by which operands are obtained must be deterministic and
 *   bounded and must not introduce scheduler policy.
 *
 * Validity, uniqueness, idempotence, and monotonicity invariants:
 *   A session may have zero or one interactive switch-operands record.
 *   A switch-operands record may be created only after an interactive
 *   execution target exists.
 *   Duplicate or conflicting preparations are invalid input.
 *   Creating preparation with identical inputs returns the existing record.
 *   In this phase, identical inputs means the same
 *   execution-target-associated session and consumer group.
 *   Preparation is a monotonic transition in this phase; once prepared, the
 *   state does not revert within this contract.
 *   Replacement or withdrawal is not defined in this phase.
 */

#include <kernul/arch.h>
#include <kernul/interactive_execution_target.h>
#include <kernul/types.h>

struct process_group;

#define INTERACTIVE_SWITCH_OPERANDS_STATE_NONE      0
#define INTERACTIVE_SWITCH_OPERANDS_STATE_PREPARED  1

struct interactive_switch_operands {
    struct interactive_execution_target *target;
    struct process_group *consumer_group;
    arch_cpu_state_t *from;
    arch_cpu_state_t *to;
    u32 state;
};

struct interactive_switch_operands *
interactive_switch_operands_prepare(struct interactive_execution_target *target);

u32
interactive_switch_operands_state(const struct interactive_switch_operands *iso);

#endif /* KERNUL_INTERACTIVE_SWITCH_OPERANDS_H */
