/*
 * core/lifecycle_assertion.c
 *
 * Caller-scoped lifecycle boundary assertion over diagnostic surfaces.
 */

#include <kernul/lifecycle_assertion.h>

lifecycle_assert_result_t
lifecycle_assert(
    const lifecycle_snapshot_t *snapshot,
    lifecycle_verify_result_t verify_result,
    const lifecycle_provenance_t *provenance,
    const lifecycle_boundary_t *boundary)
{
    bool unknown = false;
    bool violated = false;

    if (boundary == NULL)
        return LIFECYCLE_ASSERT_UNKNOWN;
    if (snapshot == NULL)
        return LIFECYCLE_ASSERT_UNKNOWN;

    if (boundary->require_verified
        && verify_result == LIFECYCLE_VERIFY_UNKNOWN) {
        unknown = true;
    }
    if (boundary->require_recovery_complete && provenance == NULL)
        unknown = true;
    if (boundary->require_retry_complete && provenance == NULL)
        unknown = true;

    if (boundary->require_finalized && !snapshot->finalized)
        violated = true;
    if (boundary->require_ready && !snapshot->ready)
        violated = true;

    if (boundary->require_verified
        && verify_result != LIFECYCLE_VERIFY_UNKNOWN
        && verify_result != LIFECYCLE_VERIFY_VERIFIED) {
        violated = true;
    }

    if (boundary->require_recovery_complete
        && provenance != NULL
        && provenance->recovery_outcome_timestamp == 0U) {
        violated = true;
    }

    if (boundary->require_retry_complete
        && provenance != NULL
        && provenance->retry_outcome_timestamp == 0U) {
        violated = true;
    }

    if (violated)
        return LIFECYCLE_ASSERT_VIOLATED;
    if (unknown)
        return LIFECYCLE_ASSERT_UNKNOWN;

    return LIFECYCLE_ASSERT_SATISFIED;
}
