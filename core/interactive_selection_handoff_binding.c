/*
 * core/interactive_selection_handoff_binding.c
 *
 * Selection-result to execution-handoff binding stubs.
 *
 * This file binds deterministic selection output to per-session handoff
 * materialization only.
 * No selection policy changes, no execution transfer, no time behavior,
 * no fairness behavior, no priority behavior, no preemption behavior, and
 * no CPU binding are implemented here.
 */

#include <kernul/interactive_selection_handoff_binding.h>
#include <kernul/interactive_selection.h>
#include <kernul/interactive_selection_result.h>
#include <kernul/interactive_execution_handoff.h>
#include <kernul/interactive_runnable.h>

struct interactive_execution_handoff *
interactive_selection_handoff_bind(struct interactive_runnable *operand)
{
    struct interactive_selection *selection;
    const struct interactive_selection_result *selection_result;

    selection = interactive_selection_select(operand);
    if (selection == NULL)
        return NULL;

    selection_result = interactive_selection_result_expose(selection);
    if (selection_result == NULL)
        return NULL;

    return interactive_execution_handoff_materialize(selection_result->session,
                                                     selection_result);
}
