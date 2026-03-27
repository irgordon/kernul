/*
 * tests/console/test_interactive_console_contract_compile.c
 *
 * Compile-check translation unit for include/kernul/interactive_console.h.
 * Pass condition: successful compilation.
 */

#include <kernul/interactive_console.h>
#include <kernul/compiler.h>

static struct interactive_console *(*const check_interactive_console_attach)(
    struct controlling_terminal *,
    struct process_group *) = interactive_console_attach;
static struct process_group *(*const check_interactive_console_consumer)(
    const struct interactive_console *) = interactive_console_consumer;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct interactive_console ic = {0};

    (void)check_interactive_console_attach;
    (void)check_interactive_console_consumer;

    (void)ic.ct;
    (void)ic.session;
    (void)ic.consumer_group;
}
