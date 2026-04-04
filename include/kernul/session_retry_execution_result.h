#ifndef KERNUL_SESSION_RETRY_EXECUTION_RESULT_H
#define KERNUL_SESSION_RETRY_EXECUTION_RESULT_H

/*
 * include/kernul/session_retry_execution_result.h
 *
 * Kernel-internal immutable retry execution result publication.
 * Phase 27, Task 1 boundary.
 *
 * This header defines durable lifecycle publication for the terminal result of
 * retry execution.
 *
 * Publication semantics:
 *   - publication is single-assignment and immutable
 *   - publication state transitions only UNSET -> SET
 *   - publication is independent of observation and interpretation
 *   - publication does not execute retries, grant authority, schedule work,
 *     or coordinate subsystems
 */

#include <kernul/types.h>

struct session;

typedef enum session_retry_execution_result {
    SESSION_RETRY_EXEC_OK = 0,
    SESSION_RETRY_EXEC_NOT_READY,
    SESSION_RETRY_EXEC_NOT_AUTHORIZED,
    SESSION_RETRY_EXEC_FAILED,
} session_retry_execution_result_t;

typedef enum session_retry_execution_result_state {
    SESSION_RETRY_EXEC_RESULT_UNSET = 0,
    SESSION_RETRY_EXEC_RESULT_SET,
} session_retry_execution_result_state_t;

typedef enum session_retry_execution_result_publish_result {
    SESSION_RETRY_EXEC_RESULT_PUBLISH_OK = 0,
    SESSION_RETRY_EXEC_RESULT_PUBLISH_NOT_READY,
    SESSION_RETRY_EXEC_RESULT_PUBLISH_ALREADY_SET,
    SESSION_RETRY_EXEC_RESULT_PUBLISH_FAILED,
} session_retry_execution_result_publish_result_t;

session_retry_execution_result_publish_result_t
session_publish_retry_execution_result(
    struct session *session,
    session_retry_execution_result_t exec_result);

session_retry_execution_result_state_t
session_get_retry_execution_result_state_acquire(
    const struct session *session);

#endif /* KERNUL_SESSION_RETRY_EXECUTION_RESULT_H */
