/*
 * core/interactive_dispatch.c
 *
 * Interactive scheduler dispatch contract stubs.
 *
 * This file provides deterministic dispatch-selection visibility recording
 * only.
 * A bounded single-slot dispatch record is used in this phase.
 * No execution, no context switching, no CPU binding, no resource reservation,
 * no priority assignment, no fairness logic, no time slicing, no preemption,
 * no signal behavior, no job-control behavior, no terminal I/O behavior, and
 * no VFS interaction are implemented here.
 */

#include <kernul/interactive_dispatch.h>
#include <kernul/interactive_runnable.h>
#include <kernul/interactive_admission.h>
#include <kernul/interactive_readiness.h>
#include <kernul/interactive_activation.h>
#include <kernul/interactive_console.h>
#include <kernul/controlling_terminal.h>
#include <kernul/session.h>

static struct interactive_dispatch interactive_dispatch_slot;
static u32 interactive_dispatch_slot_live;

struct interactive_dispatch *
interactive_dispatch_select(struct interactive_runnable *runnable)
{
    u32 expected = 0U;
    struct interactive_admission *admission;
    struct interactive_readiness *readiness;
    struct interactive_activation *activation;
    struct interactive_console *console;

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

    if (runnable->consumer_group->session != console->session)
        return NULL;

    if (interactive_dispatch_slot_live != 0U) {
        if (interactive_dispatch_slot.runnable == runnable &&
            interactive_dispatch_slot.consumer_group ==
                runnable->consumer_group) {
            return &interactive_dispatch_slot;
        }

        return NULL;
    }

    if (!__atomic_compare_exchange_n(&interactive_dispatch_slot_live,
                                     &expected,
                                     1U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        if (interactive_dispatch_slot.runnable == runnable &&
            interactive_dispatch_slot.consumer_group ==
                runnable->consumer_group) {
            return &interactive_dispatch_slot;
        }

        return NULL;
    }

    interactive_dispatch_slot.runnable = runnable;
    interactive_dispatch_slot.consumer_group = runnable->consumer_group;
    interactive_dispatch_slot.state = INTERACTIVE_DISPATCH_STATE_DISPATCHED;

    return &interactive_dispatch_slot;
}

u32
interactive_dispatch_state(const struct interactive_dispatch *id)
{
    if (id == NULL)
        return INTERACTIVE_DISPATCH_STATE_NONE;

    return id->state;
}
