#ifndef KERNUL_PROCESS_H
#define KERNUL_PROCESS_H

/*
 * include/kernul/process.h
 *
 * Kernel-internal process structure contract. Phase 5 boundary definition.
 *
 * This header defines structural process identity and relationships only.
 * It does not define address-space behavior, scheduling behavior, userspace
 * execution semantics, or process creation policy.
 *
 * Identity:
 *   process_id_t identifies a process object.
 *   PROCESS_ID_INVALID is never a valid identity.
 *   A valid PID must be unique among all live process objects.
 *   PID allocation and reuse policy are out of scope for this contract.
 *
 * Ownership and lifetime:
 *   struct process is refcounted shared state.
 *   Holders call process_get() to acquire a reference and process_put() to
 *   release it.
 *   Final-release reclamation mechanics are implementation-defined and may
 *   remain stubbed at this phase.
 *
 * Relationships:
 *   - Threads: one-to-many relationship represented structurally by threads.
 *   - Address space: declared as an optional associated pointer.
 *   - Parent/child: declared structurally by parent pointer and children
 *     container, without lifecycle or reparenting policy.
 */

#include <kernul/types.h>
#include <kernul/list.h>

struct address_space;

typedef u32 process_id_t;

#define PROCESS_ID_INVALID  ((process_id_t)0)

typedef enum {
    PROCESS_STATE_CREATED = 0,
    PROCESS_STATE_ALIVE   = 1,
    PROCESS_STATE_ZOMBIE  = 2,
    PROCESS_STATE_DEAD    = 3,
} process_state_t;

struct process {
    process_id_t             pid;
    process_state_t          state;
    u32                      refcount;
    struct process          *parent;
    list_t                   threads;
    struct address_space    *address_space;
    list_t                   children;
};

/*
 * process_alloc() - Allocate a process object with one owned reference.
 *
 * CONTEXT: process context only.
 * SLEEP: implementation-defined.
 * ALLOC: yes.
 * LOCKS: no lock required by caller.
 * OWNERSHIP: on success returns one owned reference; on failure returns NULL.
 * FAILURE: returns NULL when allocation is unavailable in this phase.
 */
struct process *process_alloc(void);

/*
 * process_get() - Acquire one shared reference.
 *
 * CONTEXT: any.
 * SLEEP: no.
 * ALLOC: no.
 * LOCKS: none required for refcount increment; concurrent increments are
 *        atomic. Caller still owns publication and object-graph locking.
 * OWNERSHIP: returns the same pointer with one additional reference.
 * FAILURE: NULL input returns NULL.
 */
struct process *process_get(struct process *proc);

/*
 * process_put() - Release one shared reference.
 *
 * CONTEXT: any.
 * SLEEP: no.
 * ALLOC: no.
 * LOCKS: none required for refcount decrement; concurrent decrements are
 *        atomic. Caller still owns publication and object-graph locking.
 * OWNERSHIP: drops one reference; final-release reclamation may be stubbed.
 * FAILURE: NULL input is ignored.
 */
void process_put(struct process *proc);

#endif /* KERNUL_PROCESS_H */
