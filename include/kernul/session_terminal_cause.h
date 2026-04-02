#ifndef KERNUL_SESSION_TERMINAL_CAUSE_H
#define KERNUL_SESSION_TERMINAL_CAUSE_H

/*
 * include/kernul/session_terminal_cause.h
 *
 * Kernel-internal terminal session cause classification policy.
 * Phase 14, Task 2 boundary.
 *
 * This header defines one declarative lifecycle policy:
 * when a session is already terminal, publish a descriptive terminal cause.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Guard and dependency rules:
 *   Terminal cause publication is terminal-first only.
 *   session_publish_terminal_cause_if_terminal() must observe
 *   session_is_terminal_acquire(const struct session *) == true before
 *   publication; otherwise it is a no-op.
 *   This surface may derive classification from terminal state and
 *   acknowledgment gates only.
 *
 * Informational-only semantics:
 *   Terminal cause classification is descriptive only.
 *   It must not influence initiation permission, terminal publication,
 *   cleanup, retry/restart, scheduling, selection, or any control path.
 *
 * Monotonicity and lifetime:
 *   Exactly one terminal cause may be published.
 *   Cause publication is one-way, session-owned, irreversible, and has
 *   no reset/override path for the session lifetime.
 *
 * Visibility:
 *   Publication uses explicit release semantics.
 *   Observation uses explicit acquire semantics.
 */

#include <kernul/types.h>

struct session;

enum session_terminal_cause {
    SESSION_TERMINAL_CAUSE_UNSPECIFIED = 0,
    SESSION_TERMINAL_CAUSE_COMPLETED,
    SESSION_TERMINAL_CAUSE_FAILED,
};

enum session_terminal_cause
session_get_terminal_cause_acquire(const struct session *session);

void
session_publish_terminal_cause_if_terminal(
    struct session *session,
    enum session_terminal_cause cause);

#endif /* KERNUL_SESSION_TERMINAL_CAUSE_H */
