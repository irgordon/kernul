#ifndef KERNUL_INTERACTIVE_EXECUTION_OUTCOME_AGGREGATE_H
#define KERNUL_INTERACTIVE_EXECUTION_OUTCOME_AGGREGATE_H

/*
 * include/kernul/interactive_execution_outcome_aggregate.h
 *
 * Kernel-internal per-session interactive execution outcome aggregation
 * contract. Phase 11, Task 3 boundary.
 *
 * This header defines passive, read-only, per-session outcome aggregation.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Scope and acquire semantics:
 *   interactive_execution_outcome_aggregate_acquire() observes exactly one
 *   session and consumes outcome state only through
 *   interactive_execution_outcome_view_acquire().
 *   No direct session outcome field reads are permitted by this surface.
 *
 * Counting semantics:
 *   Because the outcome record is singular per session:
 *     completed_count is in {0, 1}
 *     failed_count is in {0, 1}
 *   Unpublished outcome returns zeroed counts.
 *
 * Output semantics:
 *   The aggregate is returned by value.
 *   No allocation, no mutation, and no persistence are introduced.
 *   No repeated attempts, no fallback attempts, no policy interpretation, and
 *   no feedback behavior are introduced.
 */

#include <kernul/types.h>

struct session;

struct interactive_execution_outcome_aggregate {
    u32 completed_count;
    u32 failed_count;
};

struct interactive_execution_outcome_aggregate
interactive_execution_outcome_aggregate_acquire(struct session *session);

#endif /* KERNUL_INTERACTIVE_EXECUTION_OUTCOME_AGGREGATE_H */
