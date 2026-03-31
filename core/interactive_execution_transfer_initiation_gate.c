/*
 * core/interactive_execution_transfer_initiation_gate.c
 *
 * Interactive execution transfer initiation gate stubs.
 *
 * This file provides a single explicit transfer initiation gate only.
 * It consumes transfer operands view structure and performs one downstream
 * transfer boundary call with transparent status passthrough.
 * No outcome observation, no time behavior, no fairness behavior,
 * no priority behavior, no preemption behavior, and no CPU binding are
 * implemented here.
 */

#include <kernul/interactive_execution_transfer_initiation_gate.h>
#include <kernul/interactive_execution_transfer_operands_view.h>
#include <kernul/interactive_execution.h>
#include <kernul/session.h>

struct interactive_execution *
interactive_execution_transfer_initiate(
    struct session *session,
    const struct interactive_execution_transfer_operands_view *operands_view
)
{
    if (session == NULL || operands_view == NULL)
        return NULL;

    if (operands_view->session != session ||
        operands_view->handoff != &session->execution_handoff ||
        operands_view->operand_identity == NULL) {
        return NULL;
    }

    return interactive_execution_transfer(
        (struct interactive_switch_operands *)operands_view->operand_identity);
}
