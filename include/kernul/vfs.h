#ifndef KERNUL_VFS_H
#define KERNUL_VFS_H

/*
 * include/kernul/vfs.h
 *
 * Kernel-internal VFS core contract surface. Phase 6 boundary definition.
 *
 * This header defines minimal open-file object shape and VFS operation
 * attachment points only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Ownership and lifetime:
 *   struct vfs_file is a refcounted kernel object.
 *   Holders call vfs_file_get() to acquire a shared reference and
 *   vfs_file_put() to release a shared reference.
 *
 * backend_handle ownership:
 *   backend_handle is owned by the backend implementation.
 *   The VFS core does not allocate, free, or interpret this pointer.
 *
 * Scope boundary:
 *   This contract does not implement fd-table integration, path resolution,
 *   filesystem behavior, device behavior, pipe behavior, terminal behavior,
 *   or read/write semantics.
 *
 * Invariant:
 *   refcount must remain greater than zero while a struct vfs_file object
 *   is reachable.
 */

#include <kernul/types.h>

typedef u32 vfs_file_id_t;
typedef size_t usize;

struct vfs_file;

struct vfs_ops {
    int (*close)(struct vfs_file *f);
    int (*read)(struct vfs_file *f, void *buf, usize len, usize *out_nread);
    int (*write)(struct vfs_file *f, const void *buf, usize len, usize *out_nwritten);
};

struct vfs_file {
    vfs_file_id_t           id;
    u32                     refcount;
    const struct vfs_ops   *ops;
    void                   *backend_handle;
};

/*
 * These functions are kernel-internal VFS entry points.
 * They are not userspace system call interfaces.
 */
struct vfs_file *vfs_file_alloc(void);
struct vfs_file *vfs_file_get(struct vfs_file *f);
void vfs_file_put(struct vfs_file *f);

int vfs_init(void);
int vfs_open(struct vfs_file **out, const char *path, u32 flags);
int vfs_close(struct vfs_file *f);
int vfs_read(struct vfs_file *f, void *buf, usize len, usize *out_nread);
int vfs_write(struct vfs_file *f, const void *buf, usize len, usize *out_nwritten);

#endif /* KERNUL_VFS_H */
