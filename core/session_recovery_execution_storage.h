#ifndef KERNUL_SESSION_RECOVERY_EXECUTION_STORAGE_H
#define KERNUL_SESSION_RECOVERY_EXECUTION_STORAGE_H

#include <kernul/types.h>
#include <kernul/session_recovery_execution.h>

struct session;

void
session_publish_recovery_execution_completion_release(
    struct session *session,
    session_recovery_execution_result_t result);

bool
session_recovery_execution_completed_acquire(const struct session *session);

session_recovery_execution_result_t
session_recovery_execution_result_acquire(const struct session *session);

#endif /* KERNUL_SESSION_RECOVERY_EXECUTION_STORAGE_H */
