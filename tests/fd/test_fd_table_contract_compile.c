/*
 * tests/fd/test_fd_table_contract_compile.c
 *
 * Compile-check translation unit for include/kernul/fd_table.h.
 * Pass condition: successful compilation.
 */

#include <kernul/fd_table.h>
#include <kernul/compiler.h>

static struct fd_table *(*const check_alloc)(void) = fd_table_alloc;
static struct fd_table *(*const check_get)(struct fd_table *) = fd_table_get;
static void (*const check_put)(struct fd_table *) = fd_table_put;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct fd_table tbl = {0};

    (void)check_alloc;
    (void)check_get;
    (void)check_put;

    (void)tbl.refcount;
    (void)tbl.owner;
    (void)tbl.slots[0].state;
}
