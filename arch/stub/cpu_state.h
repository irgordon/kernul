#ifndef ARCH_STUB_CPU_STATE_H
#define ARCH_STUB_CPU_STATE_H

/*
 * arch/stub/cpu_state.h
 *
 * Context-switch contract surface for the stub architecture target.
 * Phase 4, Task 5.
 *
 * This header is the canonical location for:
 *   - the complete definition of struct arch_cpu_state
 *   - arch_cpu_state_init()
 *   - arch_cpu_state_switch()
 *
 * Contract (applies to all architecture targets):
 *   The context-switch contract declares what state must be preserved
 *   across a switch and what each function's calling obligations are.
 *   Stub implementations satisfy the interface without hardware behavior.
 *
 * Architecture binding:
 *   thread.h includes <cpu_state.h>, resolved by the build system to
 *   this file via -Iarch/stub. A real architecture port provides its
 *   own arch/<target>/cpu_state.h with complete field definitions and
 *   correct assembly-backed implementations of the two functions below.
 *   The contract documented here applies to all ports.
 */

#include <kernul/types.h>
#include <kernul/compiler.h>

/* -------------------------------------------------------------------------
 * struct arch_cpu_state
 *
 * Architecture-owned execution state for one thread.
 *
 * State preserved across arch_cpu_state_switch():
 *   A conforming implementation must save and restore all state required
 *   for the thread to resume correctly at the instruction following the
 *   call to arch_cpu_state_switch(). This includes at minimum:
 *     - all callee-saved general-purpose registers
 *     - the stack pointer
 *     - the program counter (return address)
 *     - any thread-local or per-CPU pointer registers in use
 *
 *   Caller-saved registers are not preserved; the calling convention
 *   treats them as volatile across any function call, including a switch.
 *
 *   Floating-point and vector state preservation is architecture-defined
 *   and must be documented in the target-specific cpu_state.h.
 *
 * Stub:
 *   The stub struct carries a single placeholder field to give it
 *   non-zero size. No real register state is saved or restored.
 *   A real port replaces this struct with its register layout.
 * ---------------------------------------------------------------------- */

struct arch_cpu_state {
    u64 _placeholder;   /* stub only; real ports define register fields here */
};

/*
 * Baseline guard: verify the type has non-zero size. A zero-size struct
 * embedded in thread_t or placed in an array would produce undefined
 * layout behavior.
 *
 * This is a minimum correctness check, not a structural guarantee.
 * It does not protect alignment requirements or ABI stability.
 * Alignment correctness and field layout are the responsibility of
 * each architecture port and must be verified separately.
 */
KERN_STATIC_ASSERT(sizeof(struct arch_cpu_state) > 0,
                   "arch_cpu_state must have non-zero size");

/* -------------------------------------------------------------------------
 * arch_cpu_state_init(state, entry, arg, stack_top)
 *
 * Initialize *state so that the first call to arch_cpu_state_switch()
 * with *state as the 'next' argument begins executing entry(arg) with
 * stack_top as the initial stack pointer.
 *
 * Contract:
 *   - state must point to storage embedded in or associated with the
 *     thread being initialized. Caller owns the storage.
 *   - entry must not be NULL.
 *   - stack_top must be a valid, sufficiently large stack region.
 *     Alignment requirements are architecture-defined.
 *   - arg is passed to entry as its sole argument.
 *   - Does not allocate. Does not block. Does not acquire any lock.
 *   - Must be called before the thread is first enqueued.
 *   - Must not be called on a state that has already been switched into;
 *     reinitializing live state is undefined behavior.
 *
 * Calling context:
 *   Scheduler internals during thread creation, before sched_enqueue().
 *   No locking assumed or required.
 *
 * Stub behavior:
 *   No-op. The stub switch function does not use the state fields.
 *
 * CONTEXT: task, during thread creation
 * SLEEP:   no    ALLOC: no
 * ---------------------------------------------------------------------- */
static inline void arch_cpu_state_init(struct arch_cpu_state *state,
                                        void (*entry)(void *),
                                        void *arg,
                                        void *stack_top)
{
    (void)state;
    (void)entry;
    (void)arg;
    (void)stack_top;
    /* STUB: no register state initialized. Real port sets up the
     * initial stack frame so that the switch trampoline can call
     * entry(arg) at the correct stack pointer. */
}

/* -------------------------------------------------------------------------
 * arch_cpu_state_switch(prev, next)
 *
 * Save the current CPU execution state into *prev and restore from *next.
 * On return from this function, the CPU is executing in the context of
 * the thread that owns *next.
 *
 * Contract:
 *   - prev must point to the arch_cpu_state of the currently running
 *     thread. The caller guarantees prev is valid and writable.
 *   - next must point to the arch_cpu_state of the thread to switch to.
 *     next must have been initialized by arch_cpu_state_init() before
 *     the first switch, or by a prior arch_cpu_state_switch() save.
 *   - prev and next must not be the same pointer. Switching to oneself
 *     is a caller error.
 *   - Interrupt and locking discipline:
 *       The caller must hold the run queue lock before calling this
 *       function and must not release it until after the function returns
 *       in the restored context. This is the single requirement at this
 *       phase, where no real interrupt handler exists.
 *
 *       When interrupt-driven preemption is introduced, the requirement
 *       becomes: interrupts must be disabled AND the run queue lock must
 *       be held. The two conditions are not interchangeable. Holding the
 *       lock with interrupts enabled is insufficient on a preemptible
 *       system because an interrupt can invoke the scheduler and corrupt
 *       prev before the save is complete. Each architecture port must
 *       document its precise requirement and enforce it.
 *   - Does not allocate. Does not block.
 *   - When this function eventually returns from the perspective of the
 *     thread whose state was saved into *prev, execution resumes in the
 *     execution context represented by *prev — specifically, at the
 *     saved program counter in *prev, with the saved stack pointer and
 *     registers from *prev restored. This is the point at which
 *     arch_cpu_state_switch() was called in that thread's prior
 *     execution. The lexical call site in the source code is the same,
 *     but the CPU state is the saved state of *prev, not the state of
 *     the thread that performed the original switch into *next.
 *
 * State preservation:
 *   All callee-saved registers are saved into *prev before restoration
 *   from *next. See struct arch_cpu_state documentation above.
 *
 * Stub behavior:
 *   No-op. No registers are saved or restored. The CPU continues
 *   executing the calling thread. This is correct for a single-threaded
 *   hosted build with no real concurrency.
 *
 * CONTEXT: scheduler internals, called from sched_yield()
 * SLEEP:   no    ALLOC: no
 * ---------------------------------------------------------------------- */
static inline void arch_cpu_state_switch(struct arch_cpu_state *prev,
                                          struct arch_cpu_state *next)
{
    (void)prev;
    (void)next;
    /* STUB: no register save or restore. Real port implements this in
     * assembly: saves callee-saved registers and stack pointer into
     * *prev, loads from *next, and returns to the instruction following
     * the call site in the restored thread's context. */
}

#endif /* ARCH_STUB_CPU_STATE_H */
