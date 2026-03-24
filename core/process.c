/*
 * core/process.c
 *
 * Process lifecycle stubs for the process contract surface.
 *
 * This file provides deterministic allocation/reference helpers only.
 * No scheduler behavior, no address-space behavior, and no userspace
 * behavior is implemented here.
 */

#include <kernul/process.h>
#include <kernul/assert.h>

struct process *process_alloc(void)
{
    /*
     * Phase-5 structural stub:
     * No process-object allocator exists yet, so return fixed failure.
     */
    return NULL;
}

struct process *process_get(struct process *proc)
{
    if (proc == NULL)
        return NULL;

    (void)__atomic_add_fetch(&proc->refcount, 1U, __ATOMIC_ACQ_REL);
    return proc;
}

void process_put(struct process *proc)
{
    u32 prev;

    if (proc == NULL)
        return;

    prev = __atomic_load_n(&proc->refcount, __ATOMIC_ACQUIRE);
    ASSERT(prev != 0U);

    while (prev != 0U) {
        if (__atomic_compare_exchange_n(&proc->refcount,
                                        &prev,
                                        prev - 1U,
                                        false,
                                        __ATOMIC_ACQ_REL,
                                        __ATOMIC_ACQUIRE)) {
            break;
        }
    }
}
