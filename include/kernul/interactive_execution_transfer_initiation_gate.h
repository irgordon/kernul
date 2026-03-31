#ifndef KERNUL_INTERACTIVE_EXECUTION_TRANSFER_INITIATION_GATE_H
#define KERNUL_INTERACTIVE_EXECUTION_TRANSFER_INITIATION_GATE_H

/*
 * include/kernul/interactive_execution_transfer_initiation_gate.h
 *
 * Kernel-internal interactive execution transfer initiation gate contract.
 * Phase 10, Task 4 boundary.
 *
 * This header defines explicit transfer initiation only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Required dependencies:
 *   This contract depends on struct session,
 *   struct interactive_execution_transfer_operands_view, and the existing
 *   execution transfer boundary call surface.
 *
 * Gate semantics and scope:
 *   This surface consumes a transfer operands view and performs a single
 *   downstream transfer boundary call.
 *   It is one-way and non-observing.
 *   It does not define time, fairness, priority, preemption, or CPU binding.
 *   It does not normalize boundary status.
 */

struct session;
struct interactive_execution;
struct interactive_execution_transfer_operands_view;

struct interactive_execution *
interactive_execution_transfer_initiate(
    struct session *session,
    const struct interactive_execution_transfer_operands_view *operands_view
);

#endif /* KERNUL_INTERACTIVE_EXECUTION_TRANSFER_INITIATION_GATE_H */
