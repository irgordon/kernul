/*
 * core/interactive_console.c
 *
 * Interactive console path contract stubs.
 *
 * This file provides deterministic routing designation attachment only.
 * A bounded single-slot routing object is used in this phase.
 * No terminal I/O semantics, no buffering, no scheduler interaction, no wakeup
 * semantics, no signal behavior, no job-control behavior, and no VFS behavior
 * are implemented here.
 */

#include <kernul/interactive_console.h>
#include <kernul/controlling_terminal.h>
#include <kernul/session.h>
#include <kernul/types.h>

static struct interactive_console interactive_console_slot;
static u32 interactive_console_slot_live;

struct interactive_console *
interactive_console_attach(struct controlling_terminal *ct,
                           struct process_group *consumer_group)
{
    u32 expected = 0U;

    if (ct == NULL || consumer_group == NULL)
        return NULL;

    if (ct->session == NULL || ct->terminal == NULL)
        return NULL;

    if (consumer_group->session != ct->session)
        return NULL;

    if (!__atomic_compare_exchange_n(&interactive_console_slot_live,
                                     &expected,
                                     1U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        return NULL;
    }

    interactive_console_slot.ct = ct;
    interactive_console_slot.session = ct->session;
    interactive_console_slot.consumer_group = consumer_group;

    return &interactive_console_slot;
}

struct process_group *
interactive_console_consumer(const struct interactive_console *ic)
{
    if (ic == NULL)
        return NULL;

    return ic->consumer_group;
}
