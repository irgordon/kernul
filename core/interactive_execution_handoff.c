/*
 * core/interactive_execution_handoff.c
 *
 * Interactive execution handoff substrate materialization stubs.
 *
 * This file provides deterministic per-session handoff materialization only.
 * It packages selection identity as transfer input structure only.
 * No selection policy, no ordering policy, no execution transfer, no
 * architecture switching, no fairness logic, no priority logic, no time
 * behavior, no preemption, and no CPU binding are implemented here.
 */

#include <kernul/interactive_execution_handoff.h>
#include <kernul/interactive_selection_result.h>
#include <kernul/session.h>

struct interactive_execution_handoff *
interactive_execution_handoff_materialize(
    struct session *session,
    const struct interactive_selection_result *selection
)
{
    u32 expected = 0U;
    u32 live;

    if (session == NULL || selection == NULL)
        return NULL;

    if (selection->session != session || selection->operand_identity == NULL)
        return NULL;

    /*
     * Acquire point: observe published per-session handoff identity before
     * idempotence and conflict checks.
     */
    live = __atomic_load_n(&session->execution_handoff_live, __ATOMIC_ACQUIRE);
    if (live == 1U) {
        if (session->execution_handoff.selection == selection)
            return &session->execution_handoff;

        return NULL;
    }

    if (!__atomic_compare_exchange_n(&session->execution_handoff_live,
                                     &expected,
                                     2U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        if (__atomic_load_n(&session->execution_handoff_live,
                            __ATOMIC_ACQUIRE) == 1U &&
            session->execution_handoff.selection == selection) {
            return &session->execution_handoff;
        }

        return NULL;
    }

    session->execution_handoff.session = session;
    session->execution_handoff.selection = selection;
    session->execution_handoff.operand_identity = selection->operand_identity;

    /*
     * Publish point: release-store marks the per-session handoff record
     * visible after complete field materialization.
     */
    __atomic_store_n(&session->execution_handoff_live, 1U, __ATOMIC_RELEASE);

    return &session->execution_handoff;
}
