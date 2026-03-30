#ifndef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#define KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H

/*
 * include/kernul/interactive_scheduler_container.h
 *
 * Kernel-internal interactive scheduler runnable-container materialization
 * contract. Phase 9 boundary.
 *
 * This header defines persistent scheduler runnable-container materialization
 * only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Required dependencies:
 *   This contract depends on struct interactive_scheduler_state,
 *   struct interactive_runnable, and struct session.
 *   The container is upstream of selection and exists only as mechanical
 *   scheduler substrate.
 *
 * Container record and ownership:
 *   struct interactive_scheduler_container is a kernel-internal runnable
 *   container record.
 *   The container is owned per session in this phase.
 *   The container lifetime is bounded by the lifetime of its associated
 *   session.
 *   The container does not own scheduler state or runnable membership.
 *   @scheduler_state and @runnable are borrowed associations only.
 *   No ownership transfer is implied.
 *   No reference counting is introduced.
 *   No global or subsystem-wide container object is permitted in this phase.
 *
 * Container semantics and scope:
 *   This surface materializes persistent mechanical representation only for
 *   interactive runnable membership ordering.
 *   It preserves ordering exactly as defined by interactive runnable
 *   membership ordering.
 *   It does not define ordering.
 *   It does not select.
 *   It does not mutate runnable membership.
 *   It does not reorder, filter, normalize, or derive new membership.
 *   It does not imply fairness, priority, or time behavior.
 *   It does not prepare execution, transfer execution, or perform architecture
 *   switching.
 *
 * Determinism, idempotence, and conflict:
 *   For a given session pointer and runnable pointer identity, materialization
 *   is deterministic and idempotent.
 *   Repeated materialization with identical inputs returns the existing
 *   container record.
 *   Conflict: if a session already has a materialized container associated
 *   with a different runnable pointer, materialization returns NULL.
 *   No iteration-based ordering decisions may occur in materialization.
 *   Traversal behavior must not define policy.
 */

struct interactive_scheduler_state;
struct interactive_runnable;
struct session;

struct interactive_scheduler_container {
    struct session *session;
    struct interactive_scheduler_state *scheduler_state;
    struct interactive_runnable *runnable;
};

struct interactive_scheduler_container *
interactive_scheduler_container_materialize(
    struct interactive_scheduler_state *state
);

#endif /* KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H */
