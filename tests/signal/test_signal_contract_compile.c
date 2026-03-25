/*
 * tests/signal/test_signal_contract_compile.c
 *
 * Compile-check translation unit for include/kernul/signal.h.
 * Pass condition: successful compilation.
 */

#include <kernul/signal.h>
#include <kernul/compiler.h>

static struct signal_state *(*const check_signal_state_alloc)(struct process *) =
    signal_state_alloc;
static void (*const check_signal_state_free)(struct signal_state *) =
    signal_state_free;

static int (*const check_signal_send)(struct process *, signal_id_t) =
    signal_send;
static int (*const check_signal_block)(struct signal_state *, signal_id_t) =
    signal_block;
static int (*const check_signal_unblock)(struct signal_state *, signal_id_t) =
    signal_unblock;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct signal_set set = {0};
    struct signal_state state = {0};

    (void)check_signal_state_alloc;
    (void)check_signal_state_free;
    (void)check_signal_send;
    (void)check_signal_block;
    (void)check_signal_unblock;

    (void)set.bits[0];

    (void)state.owner;
    (void)state.pending;
    (void)state.blocked;
}
