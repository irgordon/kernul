/*
 * core/interactive_execution_outcome_record.c
 *
 * Interactive execution outcome publication record stubs.
 *
 * This file provides session-owned passive outcome publication only.
 * It mechanically maps transfer boundary return state to completed/failed,
 * publishes with explicit release visibility, and provides explicit acquire
 * observation only.
 * No retry, no backoff, no timing behavior, no fairness behavior,
 * no priority behavior, no preemption behavior, and no CPU binding are
 * implemented here.
 */

#include <kernul/interactive_execution_outcome_record.h>
#include <kernul/interactive_execution.h>
#include <kernul/session.h>

struct interactive_execution_outcome_record *
interactive_execution_outcome_record_publish_from_transfer(
    struct session *session,
    struct interactive_execution *transfer_result
)
{
    enum interactive_execution_outcome_kind kind;

    if (session == NULL)
        return NULL;

    kind = INTERACTIVE_EXECUTION_OUTCOME_FAILED;
    if (transfer_result != NULL &&
        transfer_result->state == INTERACTIVE_EXECUTION_STATE_EXECUTED) {
        kind = INTERACTIVE_EXECUTION_OUTCOME_COMPLETED;
    }

    session->execution_outcome_record.kind = kind;

    /*
     * Publish point: release-store is the final visibility transition for
     * this outcome publication attempt.
     */
    __atomic_store_n(&session->execution_outcome_record_live,
                     1U,
                     __ATOMIC_RELEASE);

    return &session->execution_outcome_record;
}

const struct interactive_execution_outcome_record *
interactive_execution_outcome_record_acquire(
    struct session *session
)
{
    if (session == NULL)
        return NULL;

    if (__atomic_load_n(&session->execution_outcome_record_live,
                        __ATOMIC_ACQUIRE) != 1U) {
        return NULL;
    }

    return &session->execution_outcome_record;
}
