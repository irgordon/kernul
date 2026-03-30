/*
 * core/interactive_scheduler_container.c
 *
 * Interactive scheduler runnable-container materialization contract stubs.
 *
 * This file provides deterministic per-session container materialization only.
 * The container is a persistent mechanical representation of interactive
 * runnable membership ordering.
 * Ordering authority belongs to interactive runnable membership ordering; this
 * file does not define ordering by traversal or iteration behavior.
 * No selection policy, no execution preparation, no execution transfer, no
 * architecture switching, no fairness logic, no priority logic, no time
 * slicing, and no preemption are implemented here.
 */

#include <kernul/interactive_scheduler_container.h>
#include <kernul/interactive_scheduler_state.h>
#include <kernul/session.h>

struct interactive_scheduler_container *
interactive_scheduler_container_materialize(
    struct interactive_scheduler_state *state
)
{
    struct session *session;
    struct interactive_runnable *runnable;
    u32 live;

    if (state == NULL)
        return NULL;

    session = state->session;
    runnable = state->runnable;

    if (session == NULL || runnable == NULL)
        return NULL;

    live = __atomic_load_n(&session->scheduler_container_live, __ATOMIC_ACQUIRE);
    if (live == 1U) {
        if (session->scheduler_container.runnable == runnable &&
            session->scheduler_container.scheduler_state == state) {
            return &session->scheduler_container;
        }

        return NULL;
    }

    session->scheduler_container.session = session;
    session->scheduler_container.scheduler_state = state;
    session->scheduler_container.runnable = runnable;
    __atomic_store_n(&session->scheduler_container_live, 1U, __ATOMIC_RELEASE);

    return &session->scheduler_container;
}
