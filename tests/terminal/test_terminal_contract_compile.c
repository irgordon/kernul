/*
 * tests/terminal/test_terminal_contract_compile.c
 *
 * Compile-check translation unit for include/kernul/terminal.h.
 * Pass condition: successful compilation.
 */

#include <kernul/terminal.h>
#include <kernul/compiler.h>

static struct terminal *(*const check_terminal_create)(
    struct session *, struct process_group *) = terminal_create;
static terminal_id_t (*const check_terminal_id)(const struct terminal *) =
    terminal_id;

static struct pty_master *(*const check_pty_master_create)(void) =
    pty_master_create;
static pty_id_t (*const check_pty_master_id)(const struct pty_master *) =
    pty_master_id;

static struct pty_pair *(*const check_pty_pair_create)(
    struct pty_master *, struct terminal *) = pty_pair_create;
static pty_id_t (*const check_pty_pair_id)(const struct pty_pair *) =
    pty_pair_id;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct terminal term = {0};
    struct pty_master master = {0};
    struct pty_pair pair = {0};
    terminal_id_t tid_min = TERMINAL_ID_MIN;
    terminal_id_t tid_max = TERMINAL_ID_MAX;
    pty_id_t pid_min = PTY_ID_MIN;
    pty_id_t pid_max = PTY_ID_MAX;

    (void)check_terminal_create;
    (void)check_terminal_id;
    (void)check_pty_master_create;
    (void)check_pty_master_id;
    (void)check_pty_pair_create;
    (void)check_pty_pair_id;

    (void)tid_min;
    (void)tid_max;
    (void)pid_min;
    (void)pid_max;

    (void)term.id;
    (void)term.session;
    (void)term.foreground_group;
    (void)term.backend_handle;

    (void)master.id;
    (void)master.backend_handle;

    (void)pair.id;
    (void)pair.master;
    (void)pair.slave;
}
