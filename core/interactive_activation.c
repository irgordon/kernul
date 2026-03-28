/*
 * core/interactive_activation.c
 *
 * Interactive process activation contract stubs.
 *
 * This file provides deterministic activation-intent recording only.
 * A bounded single-slot activation record is used in this phase.
 * No scheduler invocation, no wakeup semantics, no signal behavior, no
 * job-control behavior, no terminal I/O behavior, and no VFS interaction are
 * implemented here.
 */

#include <kernul/interactive_activation.h>
#include <kernul/interactive_console.h>
#include <kernul/controlling_terminal.h>
#include <kernul/session.h>

static struct interactive_activation interactive_activation_slot;
static u32 interactive_activation_slot_live;

struct interactive_activation *
interactive_activation_create(struct interactive_console *console)
{
    u32 expected = 0U;

    if (console == NULL)
        return NULL;

    if (console->ct == NULL || console->session == NULL ||
        console->consumer_group == NULL) {
        return NULL;
    }

    if (console->ct->session == NULL || console->ct->terminal == NULL)
        return NULL;

    if (console->ct->session != console->session)
        return NULL;

    if (console->consumer_group->session != console->session)
        return NULL;

    if (interactive_activation_slot_live != 0U) {
        if (interactive_activation_slot.console == console &&
            interactive_activation_slot.consumer_group ==
                console->consumer_group) {
            return &interactive_activation_slot;
        }

        return NULL;
    }

    if (!__atomic_compare_exchange_n(&interactive_activation_slot_live,
                                     &expected,
                                     1U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        if (interactive_activation_slot.console == console &&
            interactive_activation_slot.consumer_group ==
                console->consumer_group) {
            return &interactive_activation_slot;
        }

        return NULL;
    }

    interactive_activation_slot.console = console;
    interactive_activation_slot.consumer_group = console->consumer_group;
    interactive_activation_slot.state = INTERACTIVE_ACTIVATION_STATE_RECORDED;

    return &interactive_activation_slot;
}

u32
interactive_activation_state(const struct interactive_activation *ia)
{
    if (ia == NULL)
        return INTERACTIVE_ACTIVATION_STATE_PENDING;

    return ia->state;
}
