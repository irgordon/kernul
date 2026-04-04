#ifndef KERNUL_SESSION_RECLAMATION_H
#define KERNUL_SESSION_RECLAMATION_H

/*
 * include/kernul/session_reclamation.h
 *
 * Kernel-internal finalization-gated local resource reclamation policy.
 * Phase 17, Task 1 boundary.
 *
 * This header defines one explicit reclamation action:
 * reclaim exactly one session-owned resource identity, but only after
 * finalization is already visible.
 *
 * Guard and dependency rules:
 *   Reclamation is finalized-first only.
 *   session_reclaim_resource_if_finalized() must observe
 *   session_is_finalized_acquire(const struct session *) == true before
 *   reclamation is attempted.
 *   Reclamation is ownership-bounded and must observe
 *   session_owns_resource_acquire(const struct session *, resource_id_t)
 *   before reclamation is attempted.
 *
 * Failure and accountability rules:
 *   Failure does not reopen lifecycle transitions.
 *   Failure does not mutate ownership declarations.
 *   Ownership remains historical and observable regardless of result.
 */

#include <kernul/session_ownership.h>

struct session;

typedef enum session_reclaim_result {
    SESSION_RECLAIM_OK = 0,
    SESSION_RECLAIM_NOT_FINALIZED,
    SESSION_RECLAIM_NOT_OWNED,
    SESSION_RECLAIM_FAILED,
} session_reclaim_result_t;

session_reclaim_result_t
session_reclaim_resource_if_finalized(struct session *session,
                                      resource_id_t resource);

#endif /* KERNUL_SESSION_RECLAMATION_H */
