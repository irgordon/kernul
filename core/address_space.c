/*
 * core/address_space.c
 *
 * Address-space lifecycle stubs for the Phase 5 ownership contract.
 *
 * This file provides deterministic allocation/reference helpers only.
 * No paging behavior, mapping behavior, MMU behavior, or userspace
 * behavior is implemented here.
 */

#include <kernul/address_space.h>
#include <kernul/assert.h>

struct address_space *address_space_alloc(void)
{
    /*
     * Phase-5 structural stub:
     * No address-space allocator exists yet, so return fixed failure.
     */
    return NULL;
}

struct address_space *address_space_get(struct address_space *as)
{
    if (as == NULL)
        return NULL;

    (void)__atomic_add_fetch(&as->refcount, 1U, __ATOMIC_ACQ_REL);
    return as;
}

void address_space_put(struct address_space *as)
{
    u32 prev;

    if (as == NULL)
        return;

    prev = __atomic_load_n(&as->refcount, __ATOMIC_ACQUIRE);
    ASSERT(prev != 0U);

    while (prev != 0U) {
        if (__atomic_compare_exchange_n(&as->refcount,
                                        &prev,
                                        prev - 1U,
                                        false,
                                        __ATOMIC_ACQ_REL,
                                        __ATOMIC_ACQUIRE)) {
            break;
        }
    }
}
