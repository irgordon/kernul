/*
 * core/interactive_execution_transfer_operands_view.c
 *
 * Interactive execution transfer-input view exposure stubs.
 *
 * This file exposes a deterministic immutable view over a published
 * per-session handoff record only.
 * No selection policy, no ordering policy, no execution transfer, no
 * architecture switching, no fairness logic, no priority logic, no time
 * behavior, no preemption, and no CPU binding are implemented here.
 */

#include <kernul/interactive_execution_transfer_operands_view.h>
#include <kernul/interactive_execution_handoff.h>
#include <kernul/session.h>

const struct interactive_execution_transfer_operands_view *
interactive_execution_transfer_operands_view_expose(
    struct session *session,
    const struct interactive_execution_handoff *handoff
)
{
    u32 expected = 0U;
    u32 handoff_live;
    u32 view_live;

    if (session == NULL || handoff == NULL)
        return NULL;

    if (handoff != &session->execution_handoff)
        return NULL;

    /*
     * Acquire point: consume per-session handoff publication before reading
     * handoff fields for view exposure.
     */
    handoff_live = __atomic_load_n(&session->execution_handoff_live,
                                   __ATOMIC_ACQUIRE);
    if (handoff_live != 1U)
        return NULL;

    if (handoff->session != session ||
        handoff->selection == NULL ||
        handoff->operand_identity == NULL) {
        return NULL;
    }

    view_live = __atomic_load_n(&session->execution_transfer_operands_view_live,
                                __ATOMIC_ACQUIRE);
    if (view_live == 1U) {
        if (session->execution_transfer_operands_view.handoff == handoff)
            return &session->execution_transfer_operands_view;

        return NULL;
    }

    if (!__atomic_compare_exchange_n(&session->execution_transfer_operands_view_live,
                                     &expected,
                                     2U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        view_live = __atomic_load_n(&session->execution_transfer_operands_view_live,
                                    __ATOMIC_ACQUIRE);
        if (view_live == 1U &&
            session->execution_transfer_operands_view.handoff == handoff) {
            return &session->execution_transfer_operands_view;
        }

        return NULL;
    }

    session->execution_transfer_operands_view.session = session;
    session->execution_transfer_operands_view.handoff = handoff;
    session->execution_transfer_operands_view.operand_identity =
        handoff->operand_identity;

    __atomic_store_n(&session->execution_transfer_operands_view_live,
                     1U,
                     __ATOMIC_RELEASE);

    return &session->execution_transfer_operands_view;
}
