/*
 * tests/console/test_interactive_selection_handoff_binding_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/interactive_selection_handoff_binding.h.
 * Pass condition: successful compilation.
 */

#include <kernul/interactive_selection_handoff_binding.h>
#include <kernul/compiler.h>
#include <kernul/interactive_selection_result.h>

static struct interactive_execution_handoff *(*const check_interactive_selection_handoff_bind)(
    struct interactive_runnable *) = interactive_selection_handoff_bind;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct interactive_selection_result result = {0};

    (void)check_interactive_selection_handoff_bind;
    (void)result.session;
    (void)result.operand_identity;
}
