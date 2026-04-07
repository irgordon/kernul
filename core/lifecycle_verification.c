/*
 * core/lifecycle_verification.c
 *
 * Passive lifecycle invariant verification over introspection snapshots.
 */

#include <kernul/lifecycle_verification.h>

#define LIFECYCLE_RETRY_POLICY_ALLOWED          1
#define LIFECYCLE_RECOVERY_OUTCOME_UNSET        0
#define LIFECYCLE_RETRY_OUTCOME_UNSET           0
#define LIFECYCLE_RETRY_EXECUTION_RESULT_UNSET  0

lifecycle_verify_result_t
lifecycle_verify(const lifecycle_snapshot_t *snapshot)
{
    bool unknown = false;

    if (snapshot == NULL)
        return LIFECYCLE_VERIFY_UNKNOWN;

    if (snapshot->ready && !snapshot->finalized)
        return LIFECYCLE_VERIFY_VIOLATION;

    if (snapshot->reclaimed)
        unknown = true;

    if (snapshot->recovery_executed && !snapshot->recovery_authorized)
        return LIFECYCLE_VERIFY_VIOLATION;

    if (snapshot->recovery_outcome != LIFECYCLE_RECOVERY_OUTCOME_UNSET
        && !snapshot->recovery_executed) {
        return LIFECYCLE_VERIFY_VIOLATION;
    }

    if (snapshot->recovery_outcome != LIFECYCLE_RECOVERY_OUTCOME_UNSET)
        unknown = true;

    if (snapshot->retry_authorized
        && snapshot->retry_policy != LIFECYCLE_RETRY_POLICY_ALLOWED) {
        return LIFECYCLE_VERIFY_VIOLATION;
    }

    if (snapshot->retry_executed && !snapshot->retry_authorized)
        return LIFECYCLE_VERIFY_VIOLATION;

    if (snapshot->retry_outcome != LIFECYCLE_RETRY_OUTCOME_UNSET &&
        snapshot->retry_execution_result == LIFECYCLE_RETRY_EXECUTION_RESULT_UNSET) {
        return LIFECYCLE_VERIFY_VIOLATION;
    }

    if (snapshot->retry_outcome != LIFECYCLE_RETRY_OUTCOME_UNSET)
        unknown = true;

    if (unknown)
        return LIFECYCLE_VERIFY_UNKNOWN;

    return LIFECYCLE_VERIFY_VERIFIED;
}
