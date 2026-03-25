/*
 * core/signal.c
 *
 * Signal contract stubs for kernel-internal signal state shape.
 *
 * This file provides deterministic structural helpers only.
 * No signal delivery, no execution interruption, no scheduler interaction,
 * no wakeups, no terminal behavior, and no job-control behavior are
 * implemented here.
 */

#include <kernul/signal.h>
#include <kernul/process.h>
#include <kernul/errors.h>

static struct signal_state signal_state_slot;
static u32 signal_state_slot_live;

static bool signal_id_valid(signal_id_t sig)
{
    return (sig > 0U) && (sig < SIGNAL_MAX_IDS);
}

static void signal_set_clear_all(struct signal_set *set)
{
    u32 i;
    u32 nwords = sizeof(set->bits) / sizeof(set->bits[0]);

    for (i = 0U; i < nwords; i++)
        set->bits[i] = 0U;
}

static void signal_set_mark(struct signal_set *set, signal_id_t sig)
{
    u32 idx = sig / 32U;
    u32 bit = sig % 32U;
    set->bits[idx] |= (1U << bit);
}

static void signal_set_clear(struct signal_set *set, signal_id_t sig)
{
    u32 idx = sig / 32U;
    u32 bit = sig % 32U;
    set->bits[idx] &= ~(1U << bit);
}

static struct signal_state *signal_state_for_process(struct process *target)
{
    if (target == NULL)
        return NULL;

    if (!__atomic_load_n(&signal_state_slot_live, __ATOMIC_ACQUIRE))
        return NULL;

    if (signal_state_slot.owner != target)
        return NULL;

    return &signal_state_slot;
}

struct signal_state *signal_state_alloc(struct process *owner)
{
    u32 expected = 0U;

    if (owner == NULL)
        return NULL;

    if (!__atomic_compare_exchange_n(&signal_state_slot_live,
                                     &expected,
                                     1U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        return NULL;
    }

    signal_state_slot.owner = owner;
    signal_set_clear_all(&signal_state_slot.pending);
    signal_set_clear_all(&signal_state_slot.blocked);

    return &signal_state_slot;
}

void signal_state_free(struct signal_state *state)
{
    if (state == NULL)
        return;

    if (state != &signal_state_slot)
        return;

    signal_set_clear_all(&signal_state_slot.pending);
    signal_set_clear_all(&signal_state_slot.blocked);
    signal_state_slot.owner = NULL;
    (void)__atomic_exchange_n(&signal_state_slot_live, 0U, __ATOMIC_RELEASE);
}

int signal_send(struct process *target, signal_id_t sig)
{
    struct signal_state *state;

    if (!signal_id_valid(sig))
        return KERN_EINVAL;

    state = signal_state_for_process(target);
    if (state == NULL)
        return KERN_EINVAL;

    signal_set_mark(&state->pending, sig);
    return KERN_OK;
}

int signal_block(struct signal_state *state, signal_id_t sig)
{
    if (state == NULL || !signal_id_valid(sig))
        return KERN_EINVAL;

    signal_set_mark(&state->blocked, sig);
    return KERN_OK;
}

int signal_unblock(struct signal_state *state, signal_id_t sig)
{
    if (state == NULL || !signal_id_valid(sig))
        return KERN_EINVAL;

    signal_set_clear(&state->blocked, sig);
    return KERN_OK;
}
