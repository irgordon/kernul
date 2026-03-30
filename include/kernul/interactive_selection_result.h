#ifndef KERNUL_INTERACTIVE_SELECTION_RESULT_H
#define KERNUL_INTERACTIVE_SELECTION_RESULT_H

/*
 * include/kernul/interactive_selection_result.h
 *
 * Kernel-internal interactive selection-result identity contract.
 * Phase 10 boundary support.
 *
 * This header defines structural selection-result identity only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Selection-result record and ownership:
 *   struct interactive_selection_result is a kernel-internal identity record.
 *   The record is owned by the session associated with selection output.
 *   The record does not own session lifetime.
 *   @session is a borrowed association only.
 *   @operand_identity is a borrowed identity association only.
 *   No ownership transfer is implied.
 */

struct session;

struct interactive_selection_result {
    struct session *session;
    const void *operand_identity;
};

#endif /* KERNUL_INTERACTIVE_SELECTION_RESULT_H */
