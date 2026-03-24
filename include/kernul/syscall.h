#ifndef KERNUL_SYSCALL_H
#define KERNUL_SYSCALL_H

/*
 * include/kernul/syscall.h
 *
 * Syscall entry contract surface. Phase 5.
 *
 * This header defines the first controlled userspace-to-kernel boundary
 * shape without implementing userspace execution, trap handling, syscall
 * policy, or ABI compatibility guarantees.
 *
 * Ownership boundary:
 *   - arch_syscall_enter() is architecture-owned and implemented under
 *     arch/<target>/. It represents the trap/entry boundary from userspace.
 *   - sys_dispatch() is common-code-owned and implemented in core/. It is
 *     the kernel-internal dispatch surface called after entry state decode.
 *
 * Calling context:
 *   - arch_syscall_enter(state) is entered from architecture trap context.
 *   - sys_dispatch(nr, a0..a5) is callable from kernel context after the
 *     architecture port has captured syscall number and arguments.
 *
 * ABI boundary statement:
 *   - This contract is kernel-internal at this phase.
 *   - It does not define syscall numbering ABI stability.
 *   - It does not claim userspace compatibility.
 *   - It does not define return-to-userspace behavior.
 */

#include <kernul/compiler.h>

struct arch_cpu_state;

/*
 * arch_syscall_enter(state)
 *
 * Architecture trap-entry boundary for syscalls.
 *
 * CONTEXT: architecture trap/exception context
 * SLEEP:   no
 * ALLOC:   no
 * LOCKS:   architecture-defined
 * RETURNS: never
 */
KERN_NORETURN void arch_syscall_enter(struct arch_cpu_state *state);

/*
 * sys_dispatch(nr, a0..a5)
 *
 * Kernel-internal syscall dispatch surface.
 *
 * Arguments are passed as plain integer values. No decoding helpers,
 * policy tables, or ABI adaptation are implied by this declaration.
 *
 * CONTEXT: kernel context after syscall entry capture
 * SLEEP:   architecture/subsystem-defined in later phases
 * ALLOC:   implementation-defined in later phases
 * LOCKS:   none required by this contract surface
 * RETURNS: syscall result as signed long
 */
long sys_dispatch(long nr,
                  long a0, long a1, long a2,
                  long a3, long a4, long a5);

#endif /* KERNUL_SYSCALL_H */
