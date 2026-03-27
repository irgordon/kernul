/*
 * core/controlling_terminal.c
 *
 * Controlling terminal attachment contract stubs.
 *
 * This file provides deterministic structural attachment only.
 * A bounded single-slot attachment object is used in this phase.
 * No signal behavior, no job-control behavior, no terminal I/O behavior, no
 * scheduler interaction, and no VFS or fd-table wiring are implemented here.
 */

#include <kernul/controlling_terminal.h>
#include <kernul/types.h>

static struct controlling_terminal controlling_terminal_slot;
static unsigned int controlling_terminal_slot_live;

struct controlling_terminal *
controlling_terminal_attach(struct session *session,
                            struct terminal *terminal)
{
    unsigned int expected = 0U;

    if (session == NULL || terminal == NULL)
        return NULL;

    if (!__atomic_compare_exchange_n(&controlling_terminal_slot_live,
                                     &expected,
                                     1U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        return NULL;
    }

    controlling_terminal_slot.session = session;
    controlling_terminal_slot.terminal = terminal;

    return &controlling_terminal_slot;
}

struct terminal *
controlling_terminal_get(const struct controlling_terminal *ct)
{
    if (ct == NULL)
        return NULL;

    return ct->terminal;
}
