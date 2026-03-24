/*
 * tests/process/test_process_contract_compile.c
 *
 * Compile-check translation unit for include/kernul/process.h.
 * Pass condition: successful compilation.
 */

#include <kernul/process.h>

static struct process *(*const check_alloc)(void) = process_alloc;
static struct process *(*const check_get)(struct process *) = process_get;
static void (*const check_put)(struct process *) = process_put;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct process p = {0};

    (void)check_alloc;
    (void)check_get;
    (void)check_put;

    (void)p.pid;
    (void)p.state;
    (void)p.refcount;
    (void)p.parent;
    (void)p.threads;
    (void)p.address_space;
    (void)p.children;
}
