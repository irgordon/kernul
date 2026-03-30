/*
 * core/interactive_scheduler_state.c
 *
 * Interactive scheduler-state materialization contract stubs.
 *
 * This file provides deterministic scheduler-state materialization only.
 * A bounded single-slot scheduler-state record is used in this phase.
 * The bounded single-slot model is a deterministic stand-in for the
 * per-session single-state invariant and does not define allocation policy.
 * Ordering authority is interactive runnable membership ordering; this file
 * does not define ordering by traversal or iteration behavior.
 * No selection policy, no dispatch behavior, no execution-target preparation,
 * no switch-operand preparation, no execution transfer, no architecture
 * switching, no fairness logic, no time slicing, no preemption, no run-queue
 * management, no signal behavior, no job-control behavior, and no terminal I/O
 * behavior are implemented here.
 */

#include <kernul/interactive_scheduler_state.h>
#include <kernul/interactive_runnable.h>
#include <kernul/session.h>

static struct interactive_scheduler_state interactive_scheduler_state_slot;
static u32 interactive_scheduler_state_slot_live;

struct interactive_scheduler_state *
interactive_scheduler_state_create(
    struct session *session,
    struct interactive_runnable *runnable
)
{
    u32 expected = 0U;

    if (session == NULL || runnable == NULL)
        return NULL;

    if (interactive_scheduler_state_slot_live != 0U) {
        if (interactive_scheduler_state_slot.session == session) {
            if (interactive_scheduler_state_slot.runnable == runnable)
                return &interactive_scheduler_state_slot;

            return NULL;
        }

        return NULL;
    }

    if (!__atomic_compare_exchange_n(&interactive_scheduler_state_slot_live,
                                     &expected,
                                     1U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        if (interactive_scheduler_state_slot.session == session &&
            interactive_scheduler_state_slot.runnable == runnable) {
            return &interactive_scheduler_state_slot;
        }

        return NULL;
    }

    interactive_scheduler_state_slot.session = session;
    interactive_scheduler_state_slot.runnable = runnable;
    interactive_scheduler_state_slot.state = INTERACTIVE_SCHEDULER_STATE_READY;

    return &interactive_scheduler_state_slot;
}

u32
interactive_scheduler_state_state(
    const struct interactive_scheduler_state *state
)
{
    if (state == NULL)
        return INTERACTIVE_SCHEDULER_STATE_NONE;

    return state->state;
}
