/*
 * tests/console/test_interactive_admission_contract_compile.c
 *
 * Compile-check translation unit for include/kernul/interactive_admission.h.
 * Pass condition: successful compilation.
 */

#include <kernul/interactive_admission.h>
#include <kernul/compiler.h>

static struct interactive_admission *(*const check_interactive_admission_admit)(
    struct interactive_readiness *) = interactive_admission_admit;
static u32 (*const check_interactive_admission_state)(
    const struct interactive_admission *) = interactive_admission_state;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct interactive_admission ia = {0};

    (void)check_interactive_admission_admit;
    (void)check_interactive_admission_state;

    (void)INTERACTIVE_ADMISSION_STATE_NONE;
    (void)INTERACTIVE_ADMISSION_STATE_ADMITTED;
    (void)ia.readiness;
    (void)ia.consumer_group;
    (void)ia.state;
}
