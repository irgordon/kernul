#ifndef KERNUL_INTERACTIVE_EXECUTION_OUTCOME_VIEW_H
#define KERNUL_INTERACTIVE_EXECUTION_OUTCOME_VIEW_H

/*
 * include/kernul/interactive_execution_outcome_view.h
 *
 * Kernel-internal interactive execution outcome view exposure contract.
 * Phase 11, Task 2 boundary.
 *
 * This header defines passive outcome view acquire exposure only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Required dependencies:
 *   This contract depends on struct session and
 *   struct interactive_execution_outcome_record publication marker semantics.
 *
 * Ownership strategy:
 *   struct interactive_execution_outcome_view is an explicitly documented
 *   alias view of the session-owned struct interactive_execution_outcome_record.
 *   The acquired pointer is session-owned storage and remains stable for the
 *   session lifetime.
 *   No allocation is introduced.
 *
 * Acquire semantics:
 *   interactive_execution_outcome_view_acquire() performs an explicit acquire
 *   load of session->execution_outcome_record_live.
 *   If unpublished, it returns NULL unchanged.
 *   If published, it returns the session-owned alias view pointer.
 *
 * Scope:
 *   No retries, no fallback reads, no feedback edges, and no policy semantics
 *   are defined here.
 */

#include <kernul/interactive_execution_outcome_record.h>

struct session;

struct interactive_execution_outcome_view {
    enum interactive_execution_outcome_kind kind;
};

const struct interactive_execution_outcome_view *
interactive_execution_outcome_view_acquire(struct session *session);

#endif /* KERNUL_INTERACTIVE_EXECUTION_OUTCOME_VIEW_H */
