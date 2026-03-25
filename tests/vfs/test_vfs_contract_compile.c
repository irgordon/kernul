/*
 * tests/vfs/test_vfs_contract_compile.c
 *
 * Compile-check translation unit for include/kernul/vfs.h.
 * Pass condition: successful compilation.
 */

#include <kernul/vfs.h>
#include <kernul/compiler.h>

static struct vfs_file *(*const check_file_alloc)(void) = vfs_file_alloc;
static struct vfs_file *(*const check_file_get)(struct vfs_file *) = vfs_file_get;
static void (*const check_file_put)(struct vfs_file *) = vfs_file_put;

static int (*const check_vfs_init)(void) = vfs_init;
static int (*const check_vfs_open)(struct vfs_file **, const char *, u32) = vfs_open;
static int (*const check_vfs_close)(struct vfs_file *) = vfs_close;
static int (*const check_vfs_read)(struct vfs_file *, void *, usize, usize *) = vfs_read;
static int (*const check_vfs_write)(struct vfs_file *, const void *, usize, usize *) = vfs_write;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct vfs_file f = {0};
    struct vfs_ops ops = {0};

    (void)check_file_alloc;
    (void)check_file_get;
    (void)check_file_put;
    (void)check_vfs_init;
    (void)check_vfs_open;
    (void)check_vfs_close;
    (void)check_vfs_read;
    (void)check_vfs_write;

    (void)f.id;
    (void)f.refcount;
    (void)f.ops;
    (void)f.backend_handle;

    (void)ops.close;
    (void)ops.read;
    (void)ops.write;
}
