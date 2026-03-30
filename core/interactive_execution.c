/*
 * core/interactive_execution.c
 *
 * Interactive execution transfer contract stubs.
 *
 * This file provides deterministic execution-transfer recording only.
 * A bounded single-slot execution record is used in this phase.
 * The bounded single-slot model is a deterministic stand-in for the
 * one-per-session invariant and does not define future allocation policy.
 * No scheduling decisions, no fairness logic, no time slicing, no preemption,
 * no signal behavior, no job-control behavior, no terminal I/O behavior, and
 * no VFS interaction are implemented here.
 */

#include <kernul/interactive_execution.h>
#include <cpu_state.h>
#include <kernul/interactive_switch_operands.h>
#include <kernul/interactive_execution_target.h>
#include <kernul/interactive_dispatch.h>
#include <kernul/interactive_runnable.h>
#include <kernul/interactive_admission.h>
#include <kernul/interactive_readiness.h>
#include <kernul/interactive_activation.h>
#include <kernul/interactive_console.h>
#include <kernul/controlling_terminal.h>
#include <kernul/session.h>

static struct interactive_execution interactive_execution_slot;
static struct session *interactive_execution_slot_session;
static u32 interactive_execution_slot_live;

struct interactive_execution *
interactive_execution_transfer(struct interactive_switch_operands *operands)
{
    u32 expected = 0U;
    struct interactive_execution_target *target;
    struct interactive_dispatch *dispatch;
    struct interactive_runnable *runnable;
    struct interactive_admission *admission;
    struct interactive_readiness *readiness;
    struct interactive_activation *activation;
    struct interactive_console *console;
    struct process_group *consumer_group;
    struct session *session;

    if (operands == NULL)
        return NULL;

    if (operands->state != INTERACTIVE_SWITCH_OPERANDS_STATE_PREPARED)
        return NULL;

    if (operands->target == NULL || operands->consumer_group == NULL)
        return NULL;

    if (operands->from == NULL || operands->to == NULL)
        return NULL;

    if (operands->from == operands->to)
        return NULL;

    target = operands->target;
    if (target->state != INTERACTIVE_EXECUTION_TARGET_STATE_PREPARED)
        return NULL;

    if (target->dispatch == NULL || target->consumer_group == NULL)
        return NULL;

    if (target->consumer_group != operands->consumer_group)
        return NULL;

    dispatch = target->dispatch;
    if (dispatch->state != INTERACTIVE_DISPATCH_STATE_DISPATCHED)
        return NULL;

    if (dispatch->runnable == NULL || dispatch->consumer_group == NULL)
        return NULL;

    if (dispatch->consumer_group != operands->consumer_group)
        return NULL;

    runnable = dispatch->runnable;
    if (runnable->state != INTERACTIVE_RUNNABLE_STATE_ENQUEUED)
        return NULL;

    if (runnable->admission == NULL || runnable->consumer_group == NULL)
        return NULL;

    if (runnable->consumer_group != operands->consumer_group)
        return NULL;

    admission = runnable->admission;
    if (admission->state != INTERACTIVE_ADMISSION_STATE_ADMITTED)
        return NULL;

    if (admission->readiness == NULL || admission->consumer_group == NULL)
        return NULL;

    if (admission->consumer_group != operands->consumer_group)
        return NULL;

    readiness = admission->readiness;
    if (readiness->state != INTERACTIVE_READINESS_STATE_READY)
        return NULL;

    if (readiness->activation == NULL || readiness->consumer_group == NULL)
        return NULL;

    if (readiness->consumer_group != operands->consumer_group)
        return NULL;

    activation = readiness->activation;
    if (activation->state != INTERACTIVE_ACTIVATION_STATE_RECORDED)
        return NULL;

    if (activation->console == NULL || activation->consumer_group == NULL)
        return NULL;

    if (activation->consumer_group != operands->consumer_group)
        return NULL;

    console = activation->console;
    if (console->ct == NULL || console->session == NULL ||
        console->consumer_group == NULL) {
        return NULL;
    }

    if (console->ct->session == NULL || console->ct->terminal == NULL)
        return NULL;

    if (console->ct->session != console->session)
        return NULL;

    if (console->consumer_group != operands->consumer_group)
        return NULL;

    consumer_group = operands->consumer_group;
    session = console->session;

    if (consumer_group->session != session)
        return NULL;

    if (interactive_execution_slot_live != 0U) {
        if (interactive_execution_slot_session == session &&
            interactive_execution_slot.consumer_group == consumer_group) {
            return &interactive_execution_slot;
        }

        return NULL;
    }

    if (!__atomic_compare_exchange_n(&interactive_execution_slot_live,
                                     &expected,
                                     1U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        if (interactive_execution_slot_session == session &&
            interactive_execution_slot.consumer_group == consumer_group) {
            return &interactive_execution_slot;
        }

        return NULL;
    }

    interactive_execution_slot.operands = operands;
    interactive_execution_slot.consumer_group = consumer_group;
    interactive_execution_slot.state = INTERACTIVE_EXECUTION_STATE_EXECUTED;
    interactive_execution_slot_session = session;

#ifndef KERNUL_STUB_TARGET
    arch_cpu_state_switch(operands->from, operands->to);
#endif

    return &interactive_execution_slot;
}
