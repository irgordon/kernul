#ifndef KERNUL_SESSION_RETRY_POLICY_H
#define KERNUL_SESSION_RETRY_POLICY_H

/*
 * include/kernul/session_retry_policy.h
 *
 * Kernel-internal immutable retry eligibility declaration policy.
 * Phase 23, Task 1 boundary.
 *
 * This header defines declarative retry eligibility meaning only.
 *
 * Policy semantics:
 *   Retry eligibility is lifecycle fact only.
 *   Declaration does not grant authority, execute retries, schedule work,
 *   coordinate subsystems, or mutate recovery outcome.
 *
 * State model:
 *   SESSION_RETRY_UNSET (initial, no declaration)
 *   SESSION_RETRY_ALLOWED
 *   SESSION_RETRY_FORBIDDEN
 *
 * Transition model:
 *   single-assignment and monotonic:
 *   UNSET -> ALLOWED
 *   UNSET -> FORBIDDEN
 *   no other transitions allowed.
 *
 * Observation rule:
 *   session_get_retry_policy_acquire() follows the centralized session
 *   observation policy in include/kernul/session.h.
 */

#include <kernul/types.h>

struct session;

typedef enum session_retry_policy {
    SESSION_RETRY_UNSET = 0,
    SESSION_RETRY_ALLOWED,
    SESSION_RETRY_FORBIDDEN,
} session_retry_policy_t;

typedef enum session_retry_policy_result {
    SESSION_RETRY_POLICY_OK = 0,
    SESSION_RETRY_POLICY_NOT_READY,
    SESSION_RETRY_POLICY_ALREADY_DECLARED,
    SESSION_RETRY_POLICY_FAILED,
} session_retry_policy_result_t;

session_retry_policy_result_t
session_declare_retry_policy(
    struct session *session,
    session_retry_policy_t policy);

session_retry_policy_t
session_get_retry_policy_acquire(const struct session *session);

#endif /* KERNUL_SESSION_RETRY_POLICY_H */
