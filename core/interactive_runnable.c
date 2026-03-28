/*
 * core/interactive_runnable.c
 *
 * Interactive runnable enqueue contract stubs.
 *
 * This file provides deterministic runnable-membership registration only.
 * A bounded single-slot runnable record is used in this phase.
 * No scheduler selection, no queue ordering, no priority assignment, no
 * fairness logic, no time slicing, no preemption, no signal behavior, no
 * job-control behavior, no terminal I/O behavior, and no VFS interaction are
 * implemented here.
 */

#include <kernul/interactive_runnable.h>
#include <kernul/interactive_admission.h>
#include <kernul/interactive_readiness.h>
#include <kernul/interactive_activation.h>
#include <kernul/interactive_console.h>
#include <kernul/controlling_terminal.h>
#include <kernul/session.h>

static struct interactive_runnable interactive_runnable_slot;
static u32 interactive_runnable_slot_live;

struct interactive_runnable *
interactive_runnable_enqueue(struct interactive_admission *admission)
{
    u32 expected = 0U;
    struct interactive_readiness *readiness;
    struct interactive_activation *activation;
    struct interactive_console *console;

    if (admission == NULL)
        return NULL;

    if (admission->state != INTERACTIVE_ADMISSION_STATE_ADMITTED)
        return NULL;

    if (admission->readiness == NULL || admission->consumer_group == NULL)
        return NULL;

    readiness = admission->readiness;
    if (readiness->state != INTERACTIVE_READINESS_STATE_READY)
        return NULL;

    if (readiness->activation == NULL || readiness->consumer_group == NULL)
        return NULL;

    if (readiness->consumer_group != admission->consumer_group)
        return NULL;

    activation = readiness->activation;
    if (activation->state != INTERACTIVE_ACTIVATION_STATE_RECORDED)
        return NULL;

    if (activation->console == NULL || activation->consumer_group == NULL)
        return NULL;

    if (activation->consumer_group != admission->consumer_group)
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

    if (console->consumer_group != admission->consumer_group)
        return NULL;

    if (admission->consumer_group->session != console->session)
        return NULL;

    if (interactive_runnable_slot_live != 0U) {
        if (interactive_runnable_slot.admission == admission &&
            interactive_runnable_slot.consumer_group ==
                admission->consumer_group) {
            return &interactive_runnable_slot;
        }

        return NULL;
    }

    if (!__atomic_compare_exchange_n(&interactive_runnable_slot_live,
                                     &expected,
                                     1U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        if (interactive_runnable_slot.admission == admission &&
            interactive_runnable_slot.consumer_group ==
                admission->consumer_group) {
            return &interactive_runnable_slot;
        }

        return NULL;
    }

    interactive_runnable_slot.admission = admission;
    interactive_runnable_slot.consumer_group = admission->consumer_group;
    interactive_runnable_slot.state = INTERACTIVE_RUNNABLE_STATE_ENQUEUED;

    return &interactive_runnable_slot;
}

u32
interactive_runnable_state(const struct interactive_runnable *ir)
{
    if (ir == NULL)
        return INTERACTIVE_RUNNABLE_STATE_NONE;

    return ir->state;
}
