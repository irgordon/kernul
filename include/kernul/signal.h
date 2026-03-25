#ifndef KERNUL_SIGNAL_H
#define KERNUL_SIGNAL_H

/*
 * include/kernul/signal.h
 *
 * Kernel-internal signal contract surface. Phase 6 boundary definition.
 *
 * This header defines minimal signal identity, set representation, and
 * process-associated signal state shape only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Signal identity rules:
 *   Signals are identified by small integer IDs only.
 *   signal_id_t value 0 is invalid and reserved as "no signal".
 *   Valid signal IDs in this phase are 1..(SIGNAL_MAX_IDS - 1).
 *   No POSIX numbering guarantees are provided.
 *   No reserved semantics beyond identity are defined.
 *   No default actions are defined in this phase.
 *
 * Ownership and lifetime:
 *   struct signal_state is owned by its associated process.
 *   signal-state lifetime is bounded by process lifetime.
 *   @owner is a borrowed pointer to the owning process object.
 *   signal_set members are embedded and not refcounted.
 *
 * Scope boundary:
 *   No signal delivery occurs in this phase.
 *   No signal handling or default actions exist in this phase.
 *   No scheduler, wakeup, terminal, or job-control behavior is defined.
 */

#include <kernul/types.h>

struct process;

typedef u32 signal_id_t;

#define SIGNAL_MAX_IDS  64U

struct signal_set {
    u32 bits[(SIGNAL_MAX_IDS + 31U) / 32U];
};

struct signal_state {
    struct process   *owner;
    struct signal_set pending;
    struct signal_set blocked;
};

struct signal_state *signal_state_alloc(struct process *owner);
void signal_state_free(struct signal_state *state);

int signal_send(struct process *target, signal_id_t sig);
int signal_block(struct signal_state *state, signal_id_t sig);
int signal_unblock(struct signal_state *state, signal_id_t sig);

#endif /* KERNUL_SIGNAL_H */
