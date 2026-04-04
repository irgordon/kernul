#ifndef KERNUL_SESSION_RECOVERY_AUTHORIZATION_H
#define KERNUL_SESSION_RECOVERY_AUTHORIZATION_H

/*
 * include/kernul/session_recovery_authorization.h
 *
 * Kernel-internal single-attempt recovery authorization policy.
 * Phase 19, Task 1 boundary.
 *
 * This header defines explicit authority to attempt recovery for a finalized
 * session that declared recovery eligibility.
 *
 * Authorization is capability-only:
 *   AUTHORIZED grants permission to attempt recovery.
 *   AUTHORIZED does not trigger execution, retries, scheduling, coordination,
 *   or lifecycle mutation.
 *
 * State model:
 *   UNAUTHORIZED -> AUTHORIZED -> CONSUMED
 *   monotonic, single-direction, and irreversible.
 *
 * Observation rule:
 *   session_is_recovery_authorized_acquire() is permissive for NULL or
 *   not-ready input and returns false.
 *   NULL/not-ready observations are non-authoritative lifecycle facts.
 */

#include <kernul/types.h>

struct session;

typedef enum session_recovery_authorization_result {
    SESSION_RECOVERY_AUTH_OK = 0,
    SESSION_RECOVERY_AUTH_NOT_FINALIZED,
    SESSION_RECOVERY_AUTH_NOT_ELIGIBLE,
    SESSION_RECOVERY_AUTH_ALREADY_USED,
    SESSION_RECOVERY_AUTH_FAILED,
} session_recovery_authorization_result_t;

typedef enum session_recovery_consume_result {
    SESSION_RECOVERY_CONSUME_OK = 0,
    SESSION_RECOVERY_CONSUME_NOT_AUTHORIZED,
    SESSION_RECOVERY_CONSUME_ALREADY_CONSUMED,
    SESSION_RECOVERY_CONSUME_FAILED,
} session_recovery_consume_result_t;

session_recovery_authorization_result_t
session_authorize_recovery_attempt(struct session *session);

session_recovery_consume_result_t
session_consume_recovery_authorization(struct session *session);

bool
session_is_recovery_authorized_acquire(const struct session *session);

#endif /* KERNUL_SESSION_RECOVERY_AUTHORIZATION_H */
