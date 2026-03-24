#ifndef KERNUL_ADDRESS_SPACE_H
#define KERNUL_ADDRESS_SPACE_H

/*
 * include/kernul/address_space.h
 *
 * Kernel-internal address-space ownership contract. Phase 5 boundary
 * definition.
 *
 * This header defines address-space identity, ownership, and process
 * association shape only.
 *
 * It does not define:
 *   - paging policy
 *   - mapping or unmapping behavior
 *   - MMU enablement behavior
 *   - loader behavior
 *   - userspace execution semantics
 *
 * Ownership model:
 *   struct address_space is shared refcounted state.
 *   Holders call address_space_get() to acquire a reference and
 *   address_space_put() to release it.
 *
 * Lifetime model:
 *   address_space_alloc() returns either NULL or one owned reference.
 *   Final-release reclamation behavior may remain stubbed in this phase.
 *
 * Process relationship:
 *   @owner references the owning process object when ownership is published.
 *   @owner may be NULL during early lifecycle before process association.
 *
 * Thread relationship:
 *   Threads are expected to execute within the address space associated with
 *   their owning process, but this contract does not enforce or validate
 *   thread/address-space attachment.
 */

#include <kernul/types.h>

struct process;
struct arch_address_space;

typedef u64 address_space_id_t;

typedef enum {
    ADDRESS_SPACE_STATE_UNINITIALIZED = 0,
    ADDRESS_SPACE_STATE_INITIALIZED   = 1,
} address_space_state_t;

struct address_space {
    address_space_id_t         id;
    u32                        refcount;
    struct process            *owner;
    address_space_state_t      state;
    struct arch_address_space *arch_handle;
};

/*
 * address_space_alloc() - Allocate an address-space object.
 *
 * CONTEXT: process context only.
 * SLEEP: implementation-defined.
 * ALLOC: yes.
 * LOCKS: no lock required by caller.
 * OWNERSHIP: on success returns one owned reference; on failure returns NULL.
 * FAILURE: returns NULL when allocation is unavailable in this phase.
 */
struct address_space *address_space_alloc(void);

/*
 * address_space_get() - Acquire one shared reference.
 *
 * CONTEXT: any.
 * SLEEP: no.
 * ALLOC: no.
 * LOCKS: none required for refcount increment; caller still owns publication
 *        and object-graph locking.
 * OWNERSHIP: returns the same pointer with one additional reference.
 * FAILURE: NULL input returns NULL.
 */
struct address_space *address_space_get(struct address_space *as);

/*
 * address_space_put() - Release one shared reference.
 *
 * CONTEXT: any.
 * SLEEP: no.
 * ALLOC: no.
 * LOCKS: none required for refcount decrement; caller still owns publication
 *        and object-graph locking.
 * OWNERSHIP: drops one reference; final-release reclamation may be stubbed.
 * FAILURE: NULL input is ignored.
 */
void address_space_put(struct address_space *as);

#endif /* KERNUL_ADDRESS_SPACE_H */
