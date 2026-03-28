#ifndef KERNUL_INTERACTIVE_READINESS_H
#define KERNUL_INTERACTIVE_READINESS_H

/*
 * include/kernul/interactive_readiness.h
 *
 * Kernel-internal interactive process readiness contract. Phase 7 boundary.
 *
 * This header defines structural readiness eligibility only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Readiness state constants:
 *   INTERACTIVE_READINESS_STATE_INACTIVE and
 *   INTERACTIVE_READINESS_STATE_READY are structural constants in this phase.
 *   No transition or scheduling semantics are defined.
 *   The readiness state field represents a single eligibility transition.
 *   Additional runtime states are defined in scheduler or execution subsystems,
 *   not in this structure.
 *
 * Readiness record and ownership:
 *   struct interactive_readiness is a kernel-internal readiness record.
 *   It is logically associated with the session of its activation; it does not
 *   own or extend the lifetime of that session.
 *   The lifetime of a readiness record is bounded by the lifetime of its
 *   associated session in this phase.
 *   Destruction and cleanup semantics are not defined in this phase.
 *   The readiness record does not own struct interactive_activation or
 *   struct process_group.
 *   @activation and @consumer_group are borrowed associations only.
 *   No ownership transfer is implied.
 *   No reference counting is introduced.
 *   Creation helpers must not call interactive_activation_get(),
 *   process_group_get(), or any future equivalent.
 *   No automatic cleanup or cascading behavior is defined.
 *
 * Readiness semantics and scope:
 *   Readiness records mark scheduler-visible eligibility, not execution.
 *   Scheduler-visible means the readiness record exists as a discoverable
 *   eligibility indicator; it does not enqueue processes, alter thread state,
 *   or trigger scheduler actions.
 *   Readiness validates the interactive execution chain:
 *     session -> controlling_terminal -> interactive_console
 *     -> interactive_activation -> interactive_readiness
 *   This surface exists solely to expose eligibility to the scheduler.
 *   Readiness does not invoke the scheduler, enqueue processes, wake threads,
 *   assign priority, deliver signals, enforce foreground/background policy, or
 *   perform terminal I/O.
 *
 * Validity, uniqueness, and idempotence invariants:
 *   A session may have zero or one interactive readiness record.
 *   A readiness record may be created only after interactive activation exists.
 *   Duplicate or conflicting readiness transitions are invalid input.
 *   Creating readiness with identical inputs returns the existing readiness
 *   record.
 *   Readiness replacement is not defined in this phase.
 *   This contract defines a single readiness eligibility invariant.
 */

#include <kernul/types.h>

struct interactive_activation;
struct process_group;

#define INTERACTIVE_READINESS_STATE_INACTIVE  0U
#define INTERACTIVE_READINESS_STATE_READY     1U

struct interactive_readiness {
    struct interactive_activation *activation;
    struct process_group *consumer_group;
    u32 state;
};

struct interactive_readiness *
interactive_readiness_mark_ready(struct interactive_activation *activation);

u32
interactive_readiness_state(const struct interactive_readiness *ir);

#endif /* KERNUL_INTERACTIVE_READINESS_H */
