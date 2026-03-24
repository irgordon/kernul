/*
 * core/example_state.c
 *
 * Example style reference for KERNUL C code.
 *
 * This file is a documentation sample for naming, formatting, comments,
 * and function contract style.
 */

#include <kernul/types.h>
#include <kernul/errors.h>
#include <kernul/spinlock.h>

typedef struct example_state {
    /*
     * Immutable after initialization.
     */
    u32 id;

    /*
     * Protected by @lock.
     */
    u32 flags;

    /*
     * Serializes all mutable state in this object.
     */
    spinlock_t lock;
} example_state_t;

/*
 * example_state_set_flag() - Set a state flag under object lock.
 * @state: target object
 * @flag: flag to set
 *
 * CONTEXT: callable in any non-sleeping context permitted by the caller.
 * SLEEP: no.
 * ALLOC: no.
 * LOCKS: acquires @state->lock.
 * OWNERSHIP: borrows @state; ownership is not transferred.
 * FAILURE: returns KERN_EINVAL if @state is NULL or @flag is zero.
 */
int example_state_set_flag(example_state_t *state, u32 flag)
{
    if (state == NULL || flag == 0)
        return KERN_EINVAL;

    spin_lock(&state->lock);
    state->flags |= flag;
    spin_unlock(&state->lock);

    return KERN_OK;
}
