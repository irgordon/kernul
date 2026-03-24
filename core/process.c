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

    proc->refcount++;
    return proc;
}

void process_put(struct process *proc)
{
    if (proc == NULL)
        return;

    if (proc->refcount > 0)
        proc->refcount--;
}
