#ifndef KERNUL_FD_TABLE_H
#define KERNUL_FD_TABLE_H

/*
 * include/kernul/fd_table.h
 *
 * Kernel-internal file descriptor table contract. Phase 6 boundary definition.
 *
 * This header defines descriptor identity, ownership, and process-association
 * shape only.
 *
 * It does not define:
 *   - userspace ABI behavior
 *   - filesystem behavior
 *   - device behavior
 *   - terminal behavior
 *   - pipe behavior
 *   - read/write semantics
 *
 * Descriptor namespace:
 *   fd_t values are small non-negative integer indices into @slots.
 *   No descriptor reuse policy is defined by this contract.
 *   No close-on-exec semantics are defined by this contract.
 *   No per-descriptor flags exist beyond occupied/free state.
 *
 * Ownership model:
 *   struct fd_table is shared refcounted state.
 *   Holders call fd_table_get() to acquire a reference and fd_table_put()
 *   to release it.
 *
 * Lifetime model:
 *   fd_table_alloc() returns either NULL or one owned reference.
 *   Final-release reclamation behavior may remain stubbed in this phase.
 *
 * Process relationship:
 *   @owner is a borrowed pointer to the owning process object.
 *   The owning process publishes and clears this association.
 *   This contract does not define process lifecycle or publication policy.
 */

#include <kernul/types.h>

struct process;

typedef int fd_t;

#define FD_TABLE_MAX_FDS  64

typedef enum {
    FD_SLOT_FREE = 0,
    FD_SLOT_OCCUPIED = 1,
} fd_slot_state_t;

struct fd_slot {
    fd_slot_state_t state;
};

struct fd_table {
    u32              refcount;
    struct process  *owner;
    struct fd_slot   slots[FD_TABLE_MAX_FDS];
};

/*
 * fd_table_alloc() - Allocate a file descriptor table.
 *
 * CONTEXT: process context only.
 * SLEEP: no.
 * ALLOC: bounded internal slot only.
 * LOCKS: no lock required by caller.
 * OWNERSHIP: on success returns one owned reference; on failure returns NULL.
 * FAILURE: returns NULL when the bounded slot is unavailable.
 */
struct fd_table *fd_table_alloc(void);

/*
 * fd_table_get() - Acquire one shared reference.
 *
 * CONTEXT: any.
 * SLEEP: no.
 * ALLOC: no.
 * LOCKS: none required for refcount increment; caller still owns publication
 *        and object-graph locking.
 * OWNERSHIP: returns the same pointer with one additional reference.
 * FAILURE: NULL input returns NULL.
 */
struct fd_table *fd_table_get(struct fd_table *tbl);

/*
 * fd_table_put() - Release one shared reference.
 *
 * CONTEXT: any.
 * SLEEP: no.
 * ALLOC: no.
 * LOCKS: none required for refcount decrement; caller still owns publication
 *        and object-graph locking.
 * OWNERSHIP: drops one reference; final-release reclamation may be stubbed.
 * FAILURE: NULL input is ignored.
 */
void fd_table_put(struct fd_table *tbl);

#endif /* KERNUL_FD_TABLE_H */
