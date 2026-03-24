/*
 * tests/address_space/test_address_space_contract_compile.c
 *
 * Compile-check translation unit for include/kernul/address_space.h.
 * Pass condition: successful compilation.
 */

#include <kernul/address_space.h>
#include <kernul/compiler.h>

static struct address_space *(*const check_alloc)(void) = address_space_alloc;
static struct address_space *(*const check_get)(struct address_space *) = address_space_get;
static void (*const check_put)(struct address_space *) = address_space_put;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct address_space as = {0};

    (void)check_alloc;
    (void)check_get;
    (void)check_put;

    (void)as.id;
    (void)as.refcount;
    (void)as.owner;
    (void)as.state;
    (void)as.arch_handle;
}
