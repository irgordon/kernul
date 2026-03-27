/*
 * tests/terminal/test_controlling_terminal_contract_compile.c
 *
 * Compile-check translation unit for include/kernul/controlling_terminal.h.
 * Pass condition: successful compilation.
 */

#include <kernul/controlling_terminal.h>
#include <kernul/compiler.h>

static struct controlling_terminal *(*const check_controlling_terminal_attach)(
    struct session *, struct terminal *) = controlling_terminal_attach;
static struct terminal *(*const check_controlling_terminal_get)(
    const struct controlling_terminal *) = controlling_terminal_get;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct controlling_terminal ct = {0};

    (void)check_controlling_terminal_attach;
    (void)check_controlling_terminal_get;

    (void)ct.session;
    (void)ct.terminal;
}
