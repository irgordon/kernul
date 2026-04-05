#ifndef KERNUL_SESSION_RETRY_OUTCOME_H
#define KERNUL_SESSION_RETRY_OUTCOME_H

/*
 * include/kernul/session_retry_outcome.h
 *
 * Kernel-internal immutable retry outcome recording.
 * Phase 28, Task 1 boundary.
 *
 * This header defines durable lifecycle recording for the retry attempt
 * outcome derived from previously published retry execution result state.
 *
 * Recording semantics:
 *   - recording is single-assignment and immutable
 *   - outcome transitions only UNSET -> SUCCESS or UNSET -> FAILURE
 *   - recording is independent of observation and interpretation
 *   - recording does not execute retries, authorize retries, schedule work,
 *     coordinate subsystems, or escalate failures
 */

#include <kernul/types.h>

struct session;

typedef enum session_retry_outcome_state {
    SESSION_RETRY_OUTCOME_UNSET = 0,
    SESSION_RETRY_OUTCOME_SUCCESS,
    SESSION_RETRY_OUTCOME_FAILURE,
} session_retry_outcome_state_t;

typedef enum session_retry_outcome_record_result {
    SESSION_RETRY_OUTCOME_RECORD_OK = 0,
    SESSION_RETRY_OUTCOME_RECORD_NOT_READY,
    SESSION_RETRY_OUTCOME_RECORD_ALREADY_SET,
    SESSION_RETRY_OUTCOME_RECORD_NO_RESULT,
    SESSION_RETRY_OUTCOME_RECORD_FAILED,
} session_retry_outcome_record_result_t;

session_retry_outcome_record_result_t
session_record_retry_outcome(struct session *session);

#endif /* KERNUL_SESSION_RETRY_OUTCOME_H */
