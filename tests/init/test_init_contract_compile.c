/*
 * tests/init/test_init_contract_compile.c
 *
 * Compile-check translation unit for include/kernul/init.h.
 * Pass condition: successful compilation.
 */

#include <kernul/init.h>
#include <kernul/compiler.h>

static int (*const check_launch)(void) = init_launch;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)check_launch;
}
