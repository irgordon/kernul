#ifndef KERNUL_INTERACTIVE_EXECUTION_HANDOFF_H
#define KERNUL_INTERACTIVE_EXECUTION_HANDOFF_H

/*
 * include/kernul/interactive_execution_handoff.h
 *
 * Kernel-internal interactive execution handoff substrate contract.
 * Phase 10 boundary.
 *
 * This header defines structural handoff materialization only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Required dependencies:
 *   This contract depends on struct interactive_selection_result,
 *   struct interactive_scheduler_container,
 *   struct interactive_scheduler_state, and struct session.
 *   This surface is downstream from interactive selection and upstream from
 *   execution transfer.
 *
 * Handoff record and ownership:
 *   struct interactive_execution_handoff is a kernel-internal handoff record.
 *   The handoff record is owned per session in this phase.
 *   The handoff record lifetime is bounded by the session lifetime unless
 *   explicitly replaced by a later phase.
 *   The handoff record does not own selection output.
 *   @selection is a borrowed association only.
 *   No ownership transfer is implied.
 *   No reference counting is introduced.
 *   No global or subsystem-wide handoff object is permitted in this phase.
 *
 * Handoff semantics and scope:
 *   This surface packages selected identity into a transfer input record.
 *   It preserves selected identity exactly as provided by selection output.
 *   It does not interpret or transform selected identity.
 *   It does not define selection, ordering, fairness, priority, or preemption.
 *   It does not trigger execution transfer.
 *   It does not observe execution outcomes.
 *   It does not bind CPU or thread identity.
 *
 * Determinism, idempotence, conflict, and visibility:
 *   For a given session and identical selection-result pointer identity,
 *   materialization is deterministic and idempotent.
 *   Repeated materialization with identical selection-result pointer identity
 *   returns the existing handoff record.
 *   Conflict: if a session already has a handoff record associated with a
 *   different selection-result pointer identity, materialization returns NULL.
 *   Publication is per-session and uses explicit acquire/release semantics.
 */

struct interactive_selection_result;
struct session;

struct interactive_execution_handoff {
    struct session *session;
    const struct interactive_selection_result *selection;
    const void *operand_identity;
};

struct interactive_execution_handoff *
interactive_execution_handoff_materialize(
    struct session *session,
    const struct interactive_selection_result *selection
);

#endif /* KERNUL_INTERACTIVE_EXECUTION_HANDOFF_H */
