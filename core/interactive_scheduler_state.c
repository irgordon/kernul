/*
 * core/interactive_scheduler_state.c
 *
 * Interactive scheduler-state materialization contract stubs.
 *
 * This file provides deterministic scheduler-state materialization only.
 * A bounded single-slot scheduler-state record per session is used in this
 * phase and does not define allocation policy.
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

struct interactive_scheduler_state *
interactive_scheduler_state_create(
    struct session *session,
    struct interactive_runnable *runnable
)
{
    u32 expected = 0U;
    u32 live;

    if (session == NULL || runnable == NULL)
        return NULL;

    live = __atomic_load_n(&session->scheduler_state_live,
                           __ATOMIC_ACQUIRE);
    if (live == 1U) {
        if (session->scheduler_state.runnable == runnable)
            return &session->scheduler_state;

        return NULL;
    }

    if (!__atomic_compare_exchange_n(&session->scheduler_state_live,
                                     &expected,
                                     2U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        if (__atomic_load_n(&session->scheduler_state_live,
                            __ATOMIC_ACQUIRE) == 1U &&
            session->scheduler_state.runnable == runnable) {
            return &session->scheduler_state;
        }

        return NULL;
    }

    session->scheduler_state.session = session;
    session->scheduler_state.runnable = runnable;
    session->scheduler_state.state = INTERACTIVE_SCHEDULER_STATE_READY;
    __atomic_store_n(&session->scheduler_state_live, 1U,
                     __ATOMIC_RELEASE);

    return &session->scheduler_state;
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
