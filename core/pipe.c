/*
 * core/pipe.c
 *
 * Pipe contract stubs for kernel-internal pipe object/endpoint shape.
 *
 * This file provides deterministic lifecycle helpers and structural endpoint
 * creation only.
 * No data transfer, buffering, wakeup behavior, scheduler behavior, signal
 * behavior, terminal behavior, VFS integration, or fd-table behavior is
 * implemented here.
 */

#include <kernul/pipe.h>
#include <kernul/errors.h>
#include <kernul/assert.h>

static struct pipe_object pipe_slot;
static u32 pipe_slot_live;
static pipe_id_t pipe_id_counter;

struct pipe_object *pipe_alloc(void)
{
    u32 expected = 0U;

    if (!__atomic_compare_exchange_n(&pipe_slot_live,
                                     &expected,
                                     1U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        return NULL;
    }

    pipe_slot.id = __atomic_add_fetch(&pipe_id_counter, 1U, __ATOMIC_ACQ_REL);
    pipe_slot.refcount = 1U;
    pipe_slot.backend_handle = NULL;

    return &pipe_slot;
}

struct pipe_object *pipe_get(struct pipe_object *p)
{
    if (p == NULL)
        return NULL;

    (void)__atomic_add_fetch(&p->refcount, 1U, __ATOMIC_ACQ_REL);
    return p;
}

void pipe_put(struct pipe_object *p)
{
    u32 prev;

    if (p == NULL)
        return;

    prev = __atomic_load_n(&p->refcount, __ATOMIC_ACQUIRE);
    ASSERT(prev != 0U);

    while (prev != 0U) {
        if (__atomic_compare_exchange_n(&p->refcount,
                                        &prev,
                                        prev - 1U,
                                        false,
                                        __ATOMIC_ACQ_REL,
                                        __ATOMIC_ACQUIRE)) {
            if ((prev - 1U) == 0U)
                (void)__atomic_exchange_n(&pipe_slot_live, 0U, __ATOMIC_RELEASE);
            break;
        }
    }
}

int pipe_open_pair(struct pipe_end *read_end,
                   struct pipe_end *write_end)
{
    struct pipe_object *p;

    if (read_end == NULL || write_end == NULL)
        return KERN_EINVAL;

    p = pipe_alloc();
    if (p == NULL)
        return KERN_ENOMEM;

    p->reader_count = 1U;
    p->writer_count = 1U;

    read_end->pipe = p;
    read_end->kind = PIPE_END_READ;

    write_end->pipe = p;
    write_end->kind = PIPE_END_WRITE;

    return KERN_OK;
}
