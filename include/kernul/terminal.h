#ifndef KERNUL_TERMINAL_H
#define KERNUL_TERMINAL_H

/*
 * include/kernul/terminal.h
 *
 * Kernel-internal terminal and PTY contract surface. Phase 6 boundary.
 *
 * This header defines minimal terminal identity, PTY master identity, and
 * PTY pairing attachment vocabulary only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Identity rules:
 *   terminal_id_t and pty_id_t are structural integer identities only.
 *   Valid terminal IDs in this phase are TERMINAL_ID_MIN..TERMINAL_ID_MAX.
 *   Valid PTY IDs in this phase are PTY_ID_MIN..PTY_ID_MAX.
 *   ID validity is structural only and mechanically testable by range check.
 *   Allocation policy and reuse policy are not defined in this phase.
 *
 * Ownership and lifetime:
 *   struct terminal, struct pty_master, and struct pty_pair are
 *   kernel-internal identity objects in this phase.
 *   struct terminal does not own struct session or struct process_group.
 *   @session and @foreground_group are borrowed associations only.
 *   terminal does not own session/process-group lifetime in this phase.
 *   struct pty_pair does not own struct pty_master or struct terminal.
 *   @master and @slave are borrowed associations only.
 *   Creation helpers must not introduce hidden retention policy and must not
 *   call session_get(), process_group_get(), or any future equivalent.
 *
 * backend_handle ownership:
 *   backend_handle is opaque and backend-owned.
 *   Terminal and PTY core code do not allocate, free, or interpret it.
 *
 * Scope boundary:
 *   PTY pairing is structural association only.
 *   foreground_group is an attachment point only.
 *   No terminal I/O semantics are implemented here.
 *   No line discipline, buffering, mode flags, or signal state are defined.
 *   No signal routing or job-control behavior is implied.
 *   No terminal access policy is defined in this phase.
 *   No VFS or fd-table wiring is implemented.
 *   No read/write semantics are implemented.
 */

#include <kernul/types.h>

struct session;
struct process_group;

typedef u32 terminal_id_t;
typedef u32 pty_id_t;

#define TERMINAL_ID_MIN  ((terminal_id_t)1U)
#define TERMINAL_ID_MAX  ((terminal_id_t)0xffffffffU)

#define PTY_ID_MIN       ((pty_id_t)1U)
#define PTY_ID_MAX       ((pty_id_t)0xffffffffU)

struct terminal {
    terminal_id_t           id;
    struct session         *session;
    struct process_group   *foreground_group;
    void                   *backend_handle;
};

struct pty_master {
    pty_id_t id;
    void    *backend_handle;
};

struct pty_pair {
    pty_id_t             id;
    struct pty_master   *master;
    struct terminal     *slave;
};

struct terminal *terminal_create(struct session *session,
                                 struct process_group *foreground_group);

terminal_id_t terminal_id(const struct terminal *term);

struct pty_master *pty_master_create(void);

pty_id_t pty_master_id(const struct pty_master *master);

struct pty_pair *pty_pair_create(struct pty_master *master,
                                 struct terminal *slave);

pty_id_t pty_pair_id(const struct pty_pair *pair);

#endif /* KERNUL_TERMINAL_H */
