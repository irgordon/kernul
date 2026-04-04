#ifndef KERNUL_SESSION_TERMINAL_STATE_H
#define KERNUL_SESSION_TERMINAL_STATE_H

/*
 * include/kernul/session_terminal_state.h
 *
 * Kernel-internal initiation-forbidden-driven terminal session state policy.
 * Phase 14, Task 1 boundary.
 *
 * This header defines one declarative lifecycle policy:
 * if execution initiation is forbidden for a session, terminal session state
 * is published for that session.
 *
 * Dependency and composition rules:
 *   This surface depends only on
 *   interactive_execution_initiation_is_permitted_acquire(const struct session *).
 *   It must not consume acknowledgment gates or outcome surfaces directly.
 *
 * Terminal state meaning:
 *   terminal means no further initiation will occur for the session and the
 *   session lifecycle is complete.
 *   terminal does not imply cleanup, reclamation, retry/restart, scheduling,
 *   selection, or coordination behavior.
 *
 * Monotonicity and lifetime:
 *   Terminal state is session-owned, monotonic, and irreversible.
 *   Once terminal is published, it must never be cleared or overridden during
 *   the session lifetime; no reset path exists in this surface.
 *
 * Visibility:
 *   Publication uses explicit release semantics.
 *   Observation uses explicit acquire semantics.
 *   Observing terminal state from a NULL or not-ready session returns false
 *   and is non-authoritative.
 */

#include <kernul/types.h>

struct session;

void
session_publish_terminal_state_if_initiation_forbidden(struct session *session);

bool
session_is_terminal_acquire(const struct session *session);

#endif /* KERNUL_SESSION_TERMINAL_STATE_H */
