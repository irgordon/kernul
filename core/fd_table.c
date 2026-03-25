/*
 * core/fd_table.c
 *
 * File descriptor table lifecycle stubs for the kernel-internal contract.
 *
 * This file provides deterministic object/reference helpers only.
 * No filesystem behavior, no device behavior, no pipe behavior, and no I/O
 * behavior are implemented here.
 */

#include <kernul/fd_table.h>
#include <kernul/assert.h>

static struct fd_table fd_table_slot;
static u32 fd_table_slot_live;

struct fd_table *fd_table_alloc(void)
{
    u32 expected = 0U;
    u32 i;

    if (!__atomic_compare_exchange_n(&fd_table_slot_live,
                                     &expected,
                                     1U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        return NULL;
    }

    fd_table_slot.refcount = 1U;
    fd_table_slot.owner = NULL;

    for (i = 0U; i < FD_TABLE_MAX_FDS; i++)
        fd_table_slot.slots[i].state = FD_SLOT_FREE;

    return &fd_table_slot;
}

struct fd_table *fd_table_get(struct fd_table *tbl)
{
    if (tbl == NULL)
        return NULL;

    (void)__atomic_add_fetch(&tbl->refcount, 1U, __ATOMIC_ACQ_REL);
    return tbl;
}

void fd_table_put(struct fd_table *tbl)
{
    u32 prev;

    if (tbl == NULL)
        return;

    prev = __atomic_load_n(&tbl->refcount, __ATOMIC_ACQUIRE);
    ASSERT(prev != 0U);

    while (prev != 0U) {
        if (__atomic_compare_exchange_n(&tbl->refcount,
                                        &prev,
                                        prev - 1U,
                                        false,
                                        __ATOMIC_ACQ_REL,
                                        __ATOMIC_ACQUIRE)) {
            if ((prev - 1U) == 0U)
                (void)__atomic_exchange_n(&fd_table_slot_live, 0U, __ATOMIC_RELEASE);
            break;
        }
    }
}
