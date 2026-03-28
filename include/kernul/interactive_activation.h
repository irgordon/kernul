#ifndef KERNUL_INTERACTIVE_ACTIVATION_H
#define KERNUL_INTERACTIVE_ACTIVATION_H

/*
 * include/kernul/interactive_activation.h
 *
 * Kernel-internal interactive process activation contract. Phase 7 boundary.
 *
 * This header defines structural interactive activation intent only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Activation state constants:
 *   INTERACTIVE_ACTIVATION_STATE_PENDING and
 *   INTERACTIVE_ACTIVATION_STATE_RECORDED are structural constants in this
 *   phase.
 *   No activation state transition semantics are defined.
 *
 * Activation record and ownership:
 *   struct interactive_activation is a kernel-internal activation record.
 *   It does not own struct interactive_console or struct process_group.
 *   @console and @consumer_group are borrowed associations only.
 *   No ownership transfer is implied.
 *   No reference counting is introduced.
 *   Creation helpers must not call process_group_get(),
 *   interactive_console_get(), or any future equivalent.
 *
 * Lifetime and cleanup:
 *   In this phase, the lifetime of an activation record is bounded by the
 *   lifetime of its associated session.
 *   Destruction and cleanup semantics are not defined in this phase.
 *   No automatic cleanup or cascading behavior is defined.
 *
 * Activation semantics and scope:
 *   Activation records mark intent, not execution.
 *   Activation validates only the routing chain:
 *     session -> controlling_terminal -> interactive_console
 *     -> interactive_activation
 *   This surface bridges routing designation to execution readiness intent.
 *   Activation does not invoke the scheduler, schedule processes, deliver
 *   signals, enforce foreground/background policy, or perform terminal I/O.
 *
 * Validity and uniqueness invariants:
 *   A session may have zero or one interactive activation.
 *   An activation may be created only after an interactive console exists.
 *   Duplicate or conflicting activations are invalid input.
 *   Creating an activation with identical inputs returns the existing
 *   activation record.
 *   Activation replacement is not defined in this phase.
 *   This contract defines a single activation-intent invariant.
 */

#include <kernul/types.h>

struct interactive_console;
struct process_group;

#define INTERACTIVE_ACTIVATION_STATE_PENDING   0U
#define INTERACTIVE_ACTIVATION_STATE_RECORDED  1U

struct interactive_activation {
    struct interactive_console *console;
    struct process_group *consumer_group;
    u32 state;
};

struct interactive_activation *
interactive_activation_create(struct interactive_console *console);

u32
interactive_activation_state(const struct interactive_activation *ia);

#endif /* KERNUL_INTERACTIVE_ACTIVATION_H */
