/*
 * core/session_recovery_execution_storage.c
 *
 * Session-owned storage access for recovery execution completion visibility.
 */

#include <kernul/session.h>
#include "session_recovery_execution_storage.h"

void
session_publish_recovery_execution_completion_release(
    struct session *session,
    session_recovery_execution_result_t result)
{
    if (session == NULL)
        return;

    __atomic_store_n(&session->recovery_execution_result, (u32)result, __ATOMIC_RELAXED);
    __atomic_store_n(&session->recovery_execution_completed, 1U, __ATOMIC_RELEASE);
}

bool
session_recovery_execution_completed_acquire(const struct session *session)
{
    if (session == NULL)
        return false;

    return __atomic_load_n(&session->recovery_execution_completed,
                           __ATOMIC_ACQUIRE) == 1U;
}

session_recovery_execution_result_t
session_recovery_execution_result_acquire(const struct session *session)
{
    if (session == NULL)
        return SESSION_RECOVERY_EXEC_FAILED;

    return (session_recovery_execution_result_t)
        __atomic_load_n(&session->recovery_execution_result, __ATOMIC_ACQUIRE);
}
