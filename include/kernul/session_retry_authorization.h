#ifndef KERNUL_SESSION_RETRY_AUTHORIZATION_H
#define KERNUL_SESSION_RETRY_AUTHORIZATION_H

/*
 * include/kernul/session_retry_authorization.h
 *
 * Kernel-internal explicit, single-use retry authorization capability.
 * Phase 24, Task 1 boundary.
 *
 * This header defines authority only.
 *
 * Authorization semantics:
 *   Retry authorization is an explicit capability.
 *   Capability is single-use and monotonic.
 *   Consumption is authoritative.
 *   Capability does not execute retries, schedule work, or coordinate.
 *
 * State model:
 *   SESSION_RETRY_AUTH_NONE -> SESSION_RETRY_AUTH_GRANTED
 *   SESSION_RETRY_AUTH_GRANTED -> SESSION_RETRY_AUTH_CONSUMED
 *   monotonic, single-direction, and irreversible.
 */

#include <kernul/types.h>

struct session;

typedef enum session_retry_authorization_state {
    SESSION_RETRY_AUTH_NONE = 0,
    SESSION_RETRY_AUTH_GRANTED,
    SESSION_RETRY_AUTH_CONSUMED,
} session_retry_authorization_state_t;

typedef enum session_retry_authorization_result {
    SESSION_RETRY_AUTH_OK = 0,
    SESSION_RETRY_AUTH_NOT_READY,
    SESSION_RETRY_AUTH_POLICY_FORBIDS,
    SESSION_RETRY_AUTH_ALREADY_GRANTED,
    SESSION_RETRY_AUTH_NOT_GRANTED,
    SESSION_RETRY_AUTH_FAILED,
} session_retry_authorization_result_t;

session_retry_authorization_result_t
session_grant_retry_authorization(struct session *session);

session_retry_authorization_result_t
session_consume_retry_authorization(struct session *session);

session_retry_authorization_state_t
session_get_retry_authorization_acquire(const struct session *session);

#endif /* KERNUL_SESSION_RETRY_AUTHORIZATION_H */
