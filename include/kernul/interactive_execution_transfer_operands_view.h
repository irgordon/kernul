#ifndef KERNUL_INTERACTIVE_EXECUTION_TRANSFER_OPERANDS_VIEW_H
#define KERNUL_INTERACTIVE_EXECUTION_TRANSFER_OPERANDS_VIEW_H

/*
 * include/kernul/interactive_execution_transfer_operands_view.h
 *
 * Kernel-internal interactive execution transfer-input view contract.
 * Phase 10 boundary.
 *
 * This header defines structural transfer-input view exposure only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Required dependencies:
 *   This contract depends on struct interactive_execution_handoff and
 *   struct session.
 *   This surface is downstream from handoff publication and upstream from
 *   execution transfer.
 *
 * Transfer-input view and ownership:
 *   struct interactive_execution_transfer_operands_view is a kernel-internal
 *   immutable view over a published per-session handoff record.
 *   It does not own or extend the lifetime of the session or handoff.
 *   @session and @handoff are borrowed associations only.
 *   @operand_identity is borrowed from the handoff record.
 *   No ownership transfer is implied.
 *   No allocation is introduced.
 *
 * View semantics and scope:
 *   interactive_execution_transfer_operands_view_expose() binds a published
 *   handoff record to a transfer-input view only.
 *   It does not mutate handoff state, session state, or scheduler state.
 *   It does not define readiness, fairness, priority, time, preemption, or
 *   CPU binding.
 *   It does not trigger execution transfer and does not observe transfer
 *   outcomes.
 *
 * Visibility semantics:
 *   Exposure consumes session->execution_handoff publication using explicit
 *   acquire semantics symmetric with handoff publication release semantics.
 *
 * Determinism and idempotence:
 *   For identical session and handoff pointer identity, exposure is
 *   deterministic and idempotent.
 *   If the handoff is absent, unpublished, invalid, or not session-owned,
 *   exposure returns NULL.
 */

#include <kernul/interactive_execution_handoff.h>

struct session;

struct interactive_execution_transfer_operands_view {
    struct session *session;
    const struct interactive_execution_handoff *handoff;
    const void *operand_identity;
};

const struct interactive_execution_transfer_operands_view *
interactive_execution_transfer_operands_view_expose(
    struct session *session,
    const struct interactive_execution_handoff *handoff
);

#endif /* KERNUL_INTERACTIVE_EXECUTION_TRANSFER_OPERANDS_VIEW_H */
