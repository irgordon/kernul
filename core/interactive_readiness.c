/*
 * core/interactive_readiness.c
 *
 * Interactive process readiness contract stubs.
 *
 * This file provides deterministic readiness-eligibility recording only.
 * A bounded single-slot readiness record is used in this phase.
 * No scheduler invocation, no run-queue manipulation, no wakeup semantics, no
 * signal behavior, no job-control behavior, no terminal I/O behavior, and no
 * VFS interaction are implemented here.
 */

#include <kernul/interactive_readiness.h>
#include <kernul/interactive_activation.h>
#include <kernul/interactive_console.h>
#include <kernul/controlling_terminal.h>
#include <kernul/session.h>

static struct interactive_readiness interactive_readiness_slot;
static u32 interactive_readiness_slot_live;

struct interactive_readiness *
interactive_readiness_mark_ready(struct interactive_activation *activation)
{
    u32 expected = 0U;
    struct interactive_console *console;

    if (activation == NULL)
        return NULL;

    if (activation->state != INTERACTIVE_ACTIVATION_STATE_RECORDED)
        return NULL;

    if (activation->console == NULL || activation->consumer_group == NULL)
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

    if (console->consumer_group != activation->consumer_group)
        return NULL;

    if (activation->consumer_group->session != console->session)
        return NULL;

    if (interactive_readiness_slot_live != 0U) {
        if (interactive_readiness_slot.activation == activation &&
            interactive_readiness_slot.consumer_group ==
                activation->consumer_group) {
            return &interactive_readiness_slot;
        }

        return NULL;
    }

    if (!__atomic_compare_exchange_n(&interactive_readiness_slot_live,
                                     &expected,
                                     1U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        if (interactive_readiness_slot.activation == activation &&
            interactive_readiness_slot.consumer_group ==
                activation->consumer_group) {
            return &interactive_readiness_slot;
        }

        return NULL;
    }

    interactive_readiness_slot.activation = activation;
    interactive_readiness_slot.consumer_group = activation->consumer_group;
    interactive_readiness_slot.state = INTERACTIVE_READINESS_STATE_READY;

    return &interactive_readiness_slot;
}

u32
interactive_readiness_state(const struct interactive_readiness *ir)
{
    if (ir == NULL)
        return INTERACTIVE_READINESS_STATE_INACTIVE;

    return ir->state;
}
