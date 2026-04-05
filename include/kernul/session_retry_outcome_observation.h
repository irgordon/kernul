#ifndef KERNUL_SESSION_RETRY_OUTCOME_OBSERVATION_H
#define KERNUL_SESSION_RETRY_OUTCOME_OBSERVATION_H

/*
 * include/kernul/session_retry_outcome_observation.h
 *
 * Read-only, readiness-gated observation for durable retry outcome state.
 * Phase 29, Task 1 boundary.
 */

#include <kernul/types.h>
#include <kernul/session_retry_outcome.h>

struct session;

typedef enum session_retry_outcome_observation_result {
    SESSION_RETRY_OUTCOME_OBS_OK = 0,
    SESSION_RETRY_OUTCOME_OBS_NOT_READY,
    SESSION_RETRY_OUTCOME_OBS_FAILED,
} session_retry_outcome_observation_result_t;

session_retry_outcome_observation_result_t
session_observe_retry_outcome(
    const struct session *session,
    session_retry_outcome_state_t *out_outcome);

#endif /* KERNUL_SESSION_RETRY_OUTCOME_OBSERVATION_H */
