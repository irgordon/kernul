/*
 * core/interactive_selection.c
 *
 * Interactive scheduler selection policy contract stubs.
 *
 * This file provides deterministic selection recording only.
 * A bounded single-slot selection record is used in this phase.
 * The bounded single-slot model is a deterministic stand-in for the
 * per-session single-selection invariant and does not define allocation policy.
 * Deterministic selection order authority belongs to interactive runnable
 * membership ordering; this file reads candidate identity from per-session
 * container membership view and does not define ordering through traversal.
 * Selection here is policy-only: no execution transfer, no architecture
 * switching, no fairness logic, no time slicing, no preemption, no run-queue
 * management, no signal behavior, no job-control behavior, and no terminal I/O
 * behavior are implemented here. This surface must not invoke execution
 * transfer or architecture switching, directly or indirectly.
 */

#include <kernul/interactive_selection.h>
#include <kernul/interactive_dispatch.h>
#include <kernul/interactive_runnable.h>
#include <kernul/interactive_scheduler_state.h>
#include <kernul/interactive_scheduler_container.h>
#include <kernul/interactive_admission.h>
#include <kernul/interactive_readiness.h>
#include <kernul/interactive_activation.h>
#include <kernul/interactive_console.h>
#include <kernul/controlling_terminal.h>
#include <kernul/session.h>

static struct interactive_selection interactive_selection_slot;
static struct session *interactive_selection_slot_session;
static struct interactive_dispatch interactive_selection_dispatch_slot;
static u32 interactive_selection_slot_live;

struct interactive_selection *
interactive_selection_select(struct interactive_runnable *runnable)
{
    u32 expected = 0U;
    struct interactive_scheduler_state *scheduler_state;
    struct interactive_scheduler_container *scheduler_container;
    struct interactive_admission *admission;
    struct interactive_readiness *readiness;
    struct interactive_activation *activation;
    struct interactive_console *console;
    struct process_group *consumer_group;
    struct session *session;

    if (runnable == NULL)
        return NULL;

    if (runnable->state != INTERACTIVE_RUNNABLE_STATE_ENQUEUED)
        return NULL;

    if (runnable->admission == NULL || runnable->consumer_group == NULL)
        return NULL;

    admission = runnable->admission;
    if (admission->state != INTERACTIVE_ADMISSION_STATE_ADMITTED)
        return NULL;

    if (admission->readiness == NULL || admission->consumer_group == NULL)
        return NULL;

    if (admission->consumer_group != runnable->consumer_group)
        return NULL;

    readiness = admission->readiness;
    if (readiness->state != INTERACTIVE_READINESS_STATE_READY)
        return NULL;

    if (readiness->activation == NULL || readiness->consumer_group == NULL)
        return NULL;

    if (readiness->consumer_group != runnable->consumer_group)
        return NULL;

    activation = readiness->activation;
    if (activation->state != INTERACTIVE_ACTIVATION_STATE_RECORDED)
        return NULL;

    if (activation->console == NULL || activation->consumer_group == NULL)
        return NULL;

    if (activation->consumer_group != runnable->consumer_group)
        return NULL;

    console = activation->console;
    if (console->ct == NULL || console->session == NULL ||
        console->consumer_group == NULL) {
        return NULL;
    }

    if (console->ct->session == NULL || console->ct->terminal == NULL)
        return NULL;

    if (console->ct->session != console->session)
        return NULL;

    if (console->consumer_group != runnable->consumer_group)
        return NULL;

    consumer_group = runnable->consumer_group;
    session = console->session;

    if (consumer_group->session != session)
        return NULL;

    scheduler_state = interactive_scheduler_state_create(session, runnable);
    if (scheduler_state == NULL)
        return NULL;

    scheduler_container =
        interactive_scheduler_container_materialize(scheduler_state);
    if (scheduler_container == NULL)
        return NULL;

    /*
     * Ordering authority remains external: interactive runnable membership
     * ordering. Container traversal behavior is non-authoritative; selection
     * only reads the membership view bound to this session.
     */
    if (scheduler_container->session != session ||
        scheduler_container->scheduler_state != scheduler_state ||
        scheduler_container->runnable != runnable) {
        return NULL;
    }

    if (interactive_selection_slot_live != 0U) {
        if (interactive_selection_slot.runnable == runnable &&
            interactive_selection_slot.consumer_group == consumer_group &&
            interactive_selection_slot_session == session) {
            return &interactive_selection_slot;
        }

        return NULL;
    }

    if (!__atomic_compare_exchange_n(&interactive_selection_slot_live,
                                     &expected,
                                     1U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        if (interactive_selection_slot.runnable == runnable &&
            interactive_selection_slot.consumer_group == consumer_group &&
            interactive_selection_slot_session == session) {
            return &interactive_selection_slot;
        }

        return NULL;
    }

    interactive_selection_dispatch_slot.runnable = runnable;
    interactive_selection_dispatch_slot.consumer_group = consumer_group;
    interactive_selection_dispatch_slot.state = INTERACTIVE_DISPATCH_STATE_DISPATCHED;

    interactive_selection_slot.runnable = runnable;
    interactive_selection_slot.dispatch = &interactive_selection_dispatch_slot;
    interactive_selection_slot.consumer_group = consumer_group;
    interactive_selection_slot.state = INTERACTIVE_SELECTION_STATE_SELECTED;
    interactive_selection_slot_session = session;

    return &interactive_selection_slot;
}

u32
interactive_selection_state(const struct interactive_selection *sel)
{
    if (sel == NULL)
        return INTERACTIVE_SELECTION_STATE_NONE;

    return sel->state;
}
