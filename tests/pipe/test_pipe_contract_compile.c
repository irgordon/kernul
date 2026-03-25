/*
 * tests/pipe/test_pipe_contract_compile.c
 *
 * Compile-check translation unit for include/kernul/pipe.h.
 * Pass condition: successful compilation.
 */

#include <kernul/pipe.h>
#include <kernul/compiler.h>

static struct pipe_object *(*const check_pipe_alloc)(void) = pipe_alloc;
static struct pipe_object *(*const check_pipe_get)(struct pipe_object *) = pipe_get;
static void (*const check_pipe_put)(struct pipe_object *) = pipe_put;

static int (*const check_pipe_open_pair)(struct pipe_end *, struct pipe_end *) =
    pipe_open_pair;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct pipe_object p = {0};
    struct pipe_end e = {0};

    (void)check_pipe_alloc;
    (void)check_pipe_get;
    (void)check_pipe_put;
    (void)check_pipe_open_pair;

    (void)p.id;
    (void)p.refcount;
    (void)p.backend_handle;
    (void)p.reader_count;
    (void)p.writer_count;

    (void)e.pipe;
    (void)e.kind;
}
