#ifndef KERNUL_INTERACTIVE_SELECTION_HANDOFF_BINDING_H
#define KERNUL_INTERACTIVE_SELECTION_HANDOFF_BINDING_H

/*
 * include/kernul/interactive_selection_handoff_binding.h
 *
 * Kernel-internal selection-result to execution-handoff binding contract.
 * Phase 10, Task 2 boundary.
 *
 * This header defines binding only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Required dependencies:
 *   This contract depends on interactive_selection_select(),
 *   interactive_selection_result_expose(),
 *   interactive_execution_handoff_materialize(), and struct session.
 *
 * Binding semantics and scope:
 *   This surface obtains deterministic selection result identity for a
 *   session-associated operand and materializes per-session handoff record.
 *   It preserves selection-result pointer identity exactly.
 *   It does not reinterpret identity.
 *   It does not trigger execution transfer.
 *   It does not define time, fairness, priority, preemption, or CPU binding.
 */

struct interactive_runnable;
struct interactive_execution_handoff;

struct interactive_execution_handoff *
interactive_selection_handoff_bind(struct interactive_runnable *operand);

#endif /* KERNUL_INTERACTIVE_SELECTION_HANDOFF_BINDING_H */
