#ifndef KERNUL_INTERACTIVE_ADMISSION_H
#define KERNUL_INTERACTIVE_ADMISSION_H

/*
 * include/kernul/interactive_admission.h
 *
 * Kernel-internal interactive scheduler admission contract. Phase 7 boundary.
 *
 * This header defines structural scheduler-admission authorization only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Admission state constants:
 *   INTERACTIVE_ADMISSION_STATE_NONE and
 *   INTERACTIVE_ADMISSION_STATE_ADMITTED are structural constants in this
 *   phase.
 *   No scheduling or execution semantics are defined.
 *   Admission state represents authorization only; runtime scheduling states
 *   are defined exclusively in scheduler subsystems.
 *   The admission state field represents a single admission transition.
 *
 * Admission record and ownership:
 *   struct interactive_admission is a kernel-internal admission record.
 *   It is logically associated with the session of its readiness; it does not
 *   own or extend the lifetime of that session.
 *   The lifetime of an admission record is bounded by the lifetime of its
 *   associated session in this phase.
 *   Destruction and cleanup semantics are not defined in this phase.
 *   The admission record does not own struct interactive_readiness or
 *   struct process_group.
 *   @readiness and @consumer_group are borrowed associations only.
 *   No ownership transfer is implied.
 *   No reference counting is introduced.
 *   Creation helpers must not call interactive_readiness_get(),
 *   process_group_get(), or any future equivalent.
 *   No automatic cleanup or cascading behavior is defined.
 *
 * Admission semantics and scope:
 *   Admission records mark scheduler admission, not execution.
 *   Admitted means the consumer is authorized for scheduler consideration; it
 *   does not place the consumer into any scheduler structure or modify
 *   execution state.
 *   Admission is a monotonic transition in this phase; once admitted, the
 *   state does not revert within this contract.
 *   Admission validates the interactive execution chain:
 *     session -> controlling_terminal -> interactive_console ->
 *     interactive_activation -> interactive_readiness ->
 *     interactive_admission
 *   This surface exists solely to authorize eligibility within the scheduler
 *   domain.
 *   Admission does not invoke the scheduler, enqueue processes, wake threads,
 *   assign priority, deliver signals, enforce foreground/background policy, or
 *   perform terminal I/O.
 *
 * Validity, uniqueness, and idempotence invariants:
 *   A session may have zero or one interactive admission record.
 *   An admission record may be created only after an interactive readiness
 *   record exists.
 *   Duplicate or conflicting admissions are invalid input.
 *   Creating admission with identical inputs returns the existing admission
 *   record.
 *   Admission replacement is not defined in this phase.
 *   This contract defines a single scheduler-admission invariant.
 */

#include <kernul/types.h>

struct interactive_readiness;
struct process_group;

#define INTERACTIVE_ADMISSION_STATE_NONE      0U
#define INTERACTIVE_ADMISSION_STATE_ADMITTED  1U

struct interactive_admission {
    struct interactive_readiness *readiness;
    struct process_group *consumer_group;
    u32 state;
};

struct interactive_admission *
interactive_admission_admit(struct interactive_readiness *readiness);

u32
interactive_admission_state(const struct interactive_admission *ia);

#endif /* KERNUL_INTERACTIVE_ADMISSION_H */
