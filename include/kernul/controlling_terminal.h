#ifndef KERNUL_CONTROLLING_TERMINAL_H
#define KERNUL_CONTROLLING_TERMINAL_H

/*
 * include/kernul/controlling_terminal.h
 *
 * Kernel-internal controlling terminal attachment contract. Phase 6 boundary.
 *
 * This header defines structural controlling-terminal attachment vocabulary
 * only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Attachment object and ownership:
 *   struct controlling_terminal is a kernel-internal identity/attachment
 *   object.
 *   It does not own struct session or struct terminal.
 *   @session and @terminal are borrowed associations only.
 *   No ownership transfer is implied.
 *   No reference counting is introduced.
 *   Creation helpers must not call session_get(), terminal_get(), or any
 *   future equivalent.
 *
 * Lifetime and cleanup:
 *   In this phase, the lifetime of a controlling terminal attachment object is
 *   bounded by the lifetime of its associated session.
 *   Destruction and cleanup semantics are not defined in this phase.
 *   No automatic cleanup or cascading behavior is defined.
 *
 * Attachment invariants and scope:
 *   A session may have zero or one controlling terminal.
 *   A terminal may be attached as the controlling terminal for at most one
 *   session.
 *   This contract defines a one-to-one attachment invariant in this phase;
 *   duplicate or conflicting attachments are invalid input.
 *   This surface defines attachment only.
 *   No job-control behavior is implied.
 *   No signal routing is implied.
 *   No foreground/background semantics are implied.
 *   No terminal I/O semantics are implied.
 */

struct session;
struct terminal;

struct controlling_terminal {
    struct session *session;
    struct terminal *terminal;
};

struct controlling_terminal *
controlling_terminal_attach(struct session *session,
                            struct terminal *terminal);

struct terminal *
controlling_terminal_get(const struct controlling_terminal *ct);

#endif /* KERNUL_CONTROLLING_TERMINAL_H */
