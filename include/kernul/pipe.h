#ifndef KERNUL_PIPE_H
#define KERNUL_PIPE_H

/*
 * include/kernul/pipe.h
 *
 * Kernel-internal pipe contract surface. Phase 6 boundary definition.
 *
 * This header defines minimal pipe object and endpoint structural vocabulary
 * only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Ownership and lifetime:
 *   struct pipe_object is a refcounted kernel object.
 *   pipe_alloc() returns one owned reference or NULL.
 *   pipe_get() acquires one shared reference.
 *   pipe_put() releases one shared reference.
 *
 * backend_handle ownership:
 *   backend_handle is opaque and owned by backend implementation.
 *   The pipe core does not allocate, free, or interpret this pointer.
 *
 * Endpoint model:
 *   struct pipe_end is a structural endpoint view only.
 *   struct pipe_end does not own a reference to struct pipe_object in this
 *   phase; the pointer is borrowed only.
 *   kind is fixed for endpoint lifetime.
 *
 * Endpoint creation:
 *   pipe_open_pair() creates structural endpoints only.
 *   Exactly one read end and one write end are initialized with fixed
 *   directional identity.
 *   No descriptor installation policy and no inheritance policy are defined.
 *
 * Phase boundary:
 *   No read/write transfer semantics are implemented here.
 *   No VFS or fd-table integration is implemented here.
 *
 * Invariant:
 *   refcount must remain greater than zero while a struct pipe_object is
 *   reachable.
 */

#include <kernul/types.h>

typedef u32 pipe_id_t;

typedef enum {
    PIPE_END_READ = 0,
    PIPE_END_WRITE = 1,
} pipe_end_kind_t;

struct pipe_object {
    pipe_id_t id;
    u32 refcount;
    void *backend_handle;
    u32 reader_count;
    u32 writer_count;
};

struct pipe_end {
    struct pipe_object *pipe;
    pipe_end_kind_t kind;
};

struct pipe_object *pipe_alloc(void);
struct pipe_object *pipe_get(struct pipe_object *p);
void pipe_put(struct pipe_object *p);

int pipe_open_pair(struct pipe_end *read_end,
                   struct pipe_end *write_end);

#endif /* KERNUL_PIPE_H */
