/*
 * core/terminal.c
 *
 * Terminal and PTY contract stubs for kernel-internal identity objects.
 *
 * This file provides deterministic bounded single-slot creation and identity
 * accessors only.
 * No terminal I/O, buffering, scheduler behavior, wakeups, signal behavior,
 * job-control behavior, PTY data transfer, VFS wiring, or fd-table wiring is
 * implemented here.
 */

#include <kernul/terminal.h>

static struct terminal terminal_slot;
static struct pty_master pty_master_slot;
static struct pty_pair pty_pair_slot;

static u32 terminal_slot_live;
static u32 pty_master_slot_live;
static u32 pty_pair_slot_live;

static terminal_id_t next_terminal_id = TERMINAL_ID_MIN;
static pty_id_t next_pty_master_id = PTY_ID_MIN;
static pty_id_t next_pty_pair_id = PTY_ID_MIN;

struct terminal *terminal_create(struct session *session,
                                 struct process_group *foreground_group)
{
    u32 expected = 0U;
    terminal_id_t id;

    if (session == NULL)
        return NULL;

    if (!__atomic_compare_exchange_n(&terminal_slot_live,
                                     &expected,
                                     1U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        return NULL;
    }

    id = __atomic_fetch_add(&next_terminal_id, 1U, __ATOMIC_RELAXED);
    if (id < TERMINAL_ID_MIN || id > TERMINAL_ID_MAX)
        id = TERMINAL_ID_MIN;

    terminal_slot.id = id;
    terminal_slot.session = session;
    terminal_slot.foreground_group = foreground_group;
    terminal_slot.backend_handle = NULL;

    return &terminal_slot;
}

terminal_id_t terminal_id(const struct terminal *term)
{
    if (term == NULL)
        return 0U;

    return term->id;
}

struct pty_master *pty_master_create(void)
{
    u32 expected = 0U;
    pty_id_t id;

    if (!__atomic_compare_exchange_n(&pty_master_slot_live,
                                     &expected,
                                     1U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        return NULL;
    }

    id = __atomic_fetch_add(&next_pty_master_id, 1U, __ATOMIC_RELAXED);
    if (id < PTY_ID_MIN || id > PTY_ID_MAX)
        id = PTY_ID_MIN;

    pty_master_slot.id = id;
    pty_master_slot.backend_handle = NULL;

    return &pty_master_slot;
}

pty_id_t pty_master_id(const struct pty_master *master)
{
    if (master == NULL)
        return 0U;

    return master->id;
}

struct pty_pair *pty_pair_create(struct pty_master *master,
                                 struct terminal *slave)
{
    u32 expected = 0U;
    pty_id_t id;

    if (master == NULL || slave == NULL)
        return NULL;

    if (!__atomic_compare_exchange_n(&pty_pair_slot_live,
                                     &expected,
                                     1U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        return NULL;
    }

    id = __atomic_fetch_add(&next_pty_pair_id, 1U, __ATOMIC_RELAXED);
    if (id < PTY_ID_MIN || id > PTY_ID_MAX)
        id = PTY_ID_MIN;

    pty_pair_slot.id = id;
    pty_pair_slot.master = master;
    pty_pair_slot.slave = slave;

    return &pty_pair_slot;
}

pty_id_t pty_pair_id(const struct pty_pair *pair)
{
    if (pair == NULL)
        return 0U;

    return pair->id;
}
