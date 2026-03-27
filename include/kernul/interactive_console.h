#ifndef KERNUL_INTERACTIVE_CONSOLE_H
#define KERNUL_INTERACTIVE_CONSOLE_H

/*
 * include/kernul/interactive_console.h
 *
 * Kernel-internal interactive console path contract. Phase 7 boundary.
 *
 * This header defines structural interactive routing designation only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Routing object and ownership:
 *   struct interactive_console is a kernel-internal routing object.
 *   It does not own struct controlling_terminal, struct session, or
 *   struct process_group.
 *   @ct, @session, and @consumer_group are borrowed associations only.
 *   No ownership transfer is implied.
 *   No reference counting is introduced.
 *   Creation helpers must not call session_get(), process_group_get(), or any
 *   future equivalent.
 *
 * Lifetime and cleanup:
 *   In this phase, the lifetime of an interactive console object is bounded by
 *   the lifetime of its associated session.
 *   Destruction and cleanup semantics are not defined in this phase.
 *   No automatic cleanup or cascading behavior is defined.
 *
 * Routing semantics and scope:
 *   An interactive console defines routing designation only.
 *   It designates which process group receives interactive input.
 *   Routing direction in this phase is terminal-to-consumer only.
 *   Reverse routing or bidirectional policy is not defined.
 *   This surface defines the routing designation required for interactive
 *   execution.
 *   It does not define input parsing, output formatting, buffering, blocking
 *   behavior, scheduling policy, signal generation, or terminal I/O semantics.
 *
 * Validity and uniqueness invariants:
 *   A session may have zero or one interactive console.
 *   An interactive console must reference a valid controlling terminal
 *   attachment.
 *   An interactive console may be created only after a controlling terminal
 *   attachment exists for the session.
 *   Duplicate or conflicting interactive console attachments are invalid input.
 *   Existing interactive console attachments must not be replaced in this
 *   phase; replacement semantics are not defined.
 *   This contract defines a one-to-one routing invariant in this phase.
 */

struct controlling_terminal;
struct session;
struct process_group;

struct interactive_console {
    struct controlling_terminal *ct;
    struct session *session;
    struct process_group *consumer_group;
};

struct interactive_console *
interactive_console_attach(struct controlling_terminal *ct,
                           struct process_group *consumer_group);

struct process_group *
interactive_console_consumer(const struct interactive_console *ic);

#endif /* KERNUL_INTERACTIVE_CONSOLE_H */
