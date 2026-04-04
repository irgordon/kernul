#ifndef KERNUL_SESSION_FINALIZATION_H
#define KERNUL_SESSION_FINALIZATION_H

/*
 * include/kernul/session_finalization.h
 *
 * Kernel-internal terminal-guarded finalization publication policy.
 * Phase 15, Task 1 boundary.
 *
 * This header defines one explicit externally-triggered lifecycle action:
 * when a session is already terminal, publish a session-owned finalized marker.
 *
 * Guard and dependency rules:
 *   Finalization publication is terminal-first only.
 *   session_publish_finalized_if_terminal() must observe
 *   session_is_terminal_acquire(const struct session *) == true before
 *   publication; otherwise it is a no-op.
 *   This surface may depend only on terminal state observation.
 *
 * Finalization semantics:
 *   Finalization is single-assignment, monotonic, and irreversible for the
 *   session lifetime. Terminal does not imply finalized.
 *   Publication is explicit invocation only. No callbacks, polling, or
 *   background propagation are introduced.
 *
 * Visibility:
 *   Publication uses explicit release semantics.
 *   Observation uses explicit acquire semantics.
 *   Observing finalized state from a NULL or not-ready session returns false
 *   and is non-authoritative.
 */

#include <kernul/types.h>

struct session;

void
session_publish_finalized_if_terminal(struct session *session);

bool
session_is_finalized_acquire(const struct session *session);

#endif /* KERNUL_SESSION_FINALIZATION_H */
