/*
 * core/interactive_admission.c
 *
 * Interactive scheduler admission contract stubs.
 *
 * This file provides deterministic scheduler-admission authorization recording
 * only.
 * A bounded single-slot admission record is used in this phase.
 * No scheduler invocation, no run-queue manipulation, no wakeup semantics, no
 * signal behavior, no job-control behavior, no terminal I/O behavior, and no
 * VFS interaction are implemented here.
 */

#include <kernul/interactive_admission.h>
#include <kernul/interactive_readiness.h>
#include <kernul/interactive_activation.h>
#include <kernul/interactive_console.h>
#include <kernul/controlling_terminal.h>
#include <kernul/session.h>

static struct interactive_admission interactive_admission_slot;
static u32 interactive_admission_slot_live;

struct interactive_admission *
interactive_admission_admit(struct interactive_readiness *readiness)
{
    u32 expected = 0U;
    struct interactive_activation *activation;
    struct interactive_console *console;

    if (readiness == NULL)
        return NULL;

    if (readiness->state != INTERACTIVE_READINESS_STATE_READY)
        return NULL;

    if (readiness->activation == NULL || readiness->consumer_group == NULL)
        return NULL;

    activation = readiness->activation;
    if (activation->state != INTERACTIVE_ACTIVATION_STATE_RECORDED)
        return NULL;

    if (activation->console == NULL || activation->consumer_group == NULL)
        return NULL;

    if (activation->consumer_group != readiness->consumer_group)
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

    if (console->consumer_group != readiness->consumer_group)
        return NULL;

    if (readiness->consumer_group->session != console->session)
        return NULL;

    if (interactive_admission_slot_live != 0U) {
        if (interactive_admission_slot.readiness == readiness &&
            interactive_admission_slot.consumer_group ==
                readiness->consumer_group) {
            return &interactive_admission_slot;
        }

        return NULL;
    }

    if (!__atomic_compare_exchange_n(&interactive_admission_slot_live,
                                     &expected,
                                     1U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        if (interactive_admission_slot.readiness == readiness &&
            interactive_admission_slot.consumer_group ==
                readiness->consumer_group) {
            return &interactive_admission_slot;
        }

        return NULL;
    }

    interactive_admission_slot.readiness = readiness;
    interactive_admission_slot.consumer_group = readiness->consumer_group;
    interactive_admission_slot.state = INTERACTIVE_ADMISSION_STATE_ADMITTED;

    return &interactive_admission_slot;
}

u32
interactive_admission_state(const struct interactive_admission *ia)
{
    if (ia == NULL)
        return INTERACTIVE_ADMISSION_STATE_NONE;

    return ia->state;
}
