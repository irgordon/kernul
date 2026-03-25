/*
 * core/vfs.c
 *
 * VFS core contract stubs for kernel-internal open-file objects.
 *
 * This file provides deterministic lifecycle helpers and fixed-return VFS
 * entry points only.
 * No path parsing, no path resolution, no filesystem behavior, no device
 * behavior, no pipe behavior, no terminal behavior, and no I/O semantics are
 * implemented here.
 */

#include <kernul/vfs.h>
#include <kernul/errors.h>
#include <kernul/assert.h>

static struct vfs_file vfs_file_slot;
static u32 vfs_file_slot_live;
static vfs_file_id_t vfs_file_id_counter;

struct vfs_file *vfs_file_alloc(void)
{
    u32 expected = 0U;

    if (!__atomic_compare_exchange_n(&vfs_file_slot_live,
                                     &expected,
                                     1U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        return NULL;
    }

    vfs_file_slot.id = __atomic_add_fetch(&vfs_file_id_counter, 1U, __ATOMIC_ACQ_REL);
    vfs_file_slot.refcount = 1U;
    vfs_file_slot.ops = NULL;
    vfs_file_slot.backend_handle = NULL;

    return &vfs_file_slot;
}

struct vfs_file *vfs_file_get(struct vfs_file *f)
{
    if (f == NULL)
        return NULL;

    (void)__atomic_add_fetch(&f->refcount, 1U, __ATOMIC_ACQ_REL);
    return f;
}

void vfs_file_put(struct vfs_file *f)
{
    u32 prev;

    if (f == NULL)
        return;

    prev = __atomic_load_n(&f->refcount, __ATOMIC_ACQUIRE);
    ASSERT(prev != 0U);

    while (prev != 0U) {
        if (__atomic_compare_exchange_n(&f->refcount,
                                        &prev,
                                        prev - 1U,
                                        false,
                                        __ATOMIC_ACQ_REL,
                                        __ATOMIC_ACQUIRE)) {
            if ((prev - 1U) == 0U)
                (void)__atomic_exchange_n(&vfs_file_slot_live, 0U, __ATOMIC_RELEASE);
            break;
        }
    }
}

int vfs_init(void)
{
    return KERN_ENOSYS;
}

int vfs_open(struct vfs_file **out, const char *path, u32 flags)
{
    (void)out;
    (void)path;
    (void)flags;
    return KERN_ENOSYS;
}

int vfs_close(struct vfs_file *f)
{
    (void)f;
    return KERN_ENOSYS;
}

int vfs_read(struct vfs_file *f, void *buf, usize len, usize *out_nread)
{
    (void)f;
    (void)buf;
    (void)len;
    (void)out_nread;
    return KERN_ENOSYS;
}

int vfs_write(struct vfs_file *f, const void *buf, usize len, usize *out_nwritten)
{
    (void)f;
    (void)buf;
    (void)len;
    (void)out_nwritten;
    return KERN_ENOSYS;
}
