#ifndef KERNUL_SESSION_RECOVERY_ELIGIBILITY_H
#define KERNUL_SESSION_RECOVERY_ELIGIBILITY_H

/*
 * include/kernul/session_recovery_eligibility.h
 *
 * Kernel-internal declarative recovery eligibility policy.
 * Phase 18, Task 1 boundary.
 *
 * This header defines one informational declaration surface:
 * a finalized session may declare whether it is eligible for recovery
 * consideration.
 *
 * Policy semantics:
 *   Recovery eligibility is informational only.
 *   Declaring eligibility does not initiate recovery, retries, scheduling,
 *   coordination, or lifecycle mutation.
 *
 * State model:
 *   SESSION_RECOVERY_ELIGIBILITY_UNSET (initial, no declaration)
 *   SESSION_RECOVERY_ELIGIBLE
 *   SESSION_RECOVERY_INELIGIBLE
 *
 * Transition model:
 *   single-assignment and monotonic:
 *   UNSET -> ELIGIBLE
 *   UNSET -> INELIGIBLE
 *   no other transitions allowed.
 *
 * Dependency boundary:
 *   This surface depends only on session identity and finalized observation
 *   via session_is_finalized_acquire(const struct session *).
 *
 * Observation rule:
 *   session_get_recovery_eligibility_acquire() is permissive for NULL or
 *   not-ready input and returns SESSION_RECOVERY_ELIGIBILITY_UNSET.
 *   NULL/not-ready observations are non-authoritative lifecycle facts.
 */

#include <kernul/types.h>

struct session;

typedef enum session_recovery_eligibility {
    SESSION_RECOVERY_ELIGIBILITY_UNSET = 0,
    SESSION_RECOVERY_ELIGIBLE,
    SESSION_RECOVERY_INELIGIBLE,
} session_recovery_eligibility_t;

typedef enum session_recovery_eligibility_result {
    SESSION_RECOVERY_ELIGIBILITY_OK = 0,
    SESSION_RECOVERY_ELIGIBILITY_NOT_FINALIZED,
    SESSION_RECOVERY_ELIGIBILITY_ALREADY_SET,
    SESSION_RECOVERY_ELIGIBILITY_FAILED,
} session_recovery_eligibility_result_t;

session_recovery_eligibility_result_t
session_declare_recovery_eligibility(
    struct session *session,
    session_recovery_eligibility_t eligibility);

session_recovery_eligibility_t
session_get_recovery_eligibility_acquire(
    const struct session *session);

#endif /* KERNUL_SESSION_RECOVERY_ELIGIBILITY_H */
