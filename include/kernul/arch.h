#ifndef KERNUL_ARCH_H
#define KERNUL_ARCH_H

/*
 * include/kernul/arch.h
 *
 * Architecture boundary surface declarations. §2.9
 *
 * Three mandatory surfaces. Implemented per-architecture in arch/<target>/.
 * Common code calls through these surfaces only.
 * These surfaces must not grow into frameworks.
 */

#include <kernul/types.h>
#include <kernul/compiler.h>

/* -------------------------------------------------------------------------
 * arch_halt() §2.9.1
 *
 * Terminal execution endpoint.
 *
 * Contract:
 *   - Does not return.
 *   - Stops forward execution unconditionally.
 *   - Does not attempt recovery.
 *   - Does not allocate memory.
 *   - Does not depend on scheduler state.
 *   - May disable interrupts if required by the architecture.
 *   - May enter a processor-defined low-power halt state.
 *
 * Caller context: any. Including early boot before subsystems are live.
 * ---------------------------------------------------------------------- */
KERN_NORETURN void arch_halt(void);

/* -------------------------------------------------------------------------
 * arch_early_init() §2.9.2
 *
 * Minimal architecture bring-up. Called once from init/main.c before any
 * subsystem initialization.
 *
 * Contract:
 *   - Callable exactly once.
 *   - May be a no-op on some architecture ports.
 *   - Must not depend on the scheduler.
 *   - Must not perform dynamic allocation.
 *   - Must not assume interrupts are enabled.
 *   - Must not perform device discovery unless explicitly approved for
 *     a specific architecture port and documented there.
 *
 * Caller context: early boot, single CPU, no interrupts assumed.
 * ---------------------------------------------------------------------- */
void arch_early_init(void);

/* -------------------------------------------------------------------------
 * arch_cpu_state_t §2.9.3
 *
 * Opaque architecture-owned CPU execution state.
 *
 * Contract:
 *   - Shape defined entirely by arch/<target>/cpu_state.h.
 *   - Used by sched/ for context switching and exception handling.
 *   - Common code holds pointers; it does not allocate or inspect fields.
 *   - Not invented by unrelated subsystems.
 *
 * arch/<target>/cpu_state.h additionally declares:
 *   void arch_cpu_state_init(arch_cpu_state_t *, void (*entry)(void *),
 *                             void *arg, void *stack_top)
 *   void arch_cpu_state_switch(arch_cpu_state_t *prev,
 *                               arch_cpu_state_t *next)
 *
 * Those declarations live in the architecture header, not here, because
 * their calling conventions may differ across architectures.
 * ---------------------------------------------------------------------- */
typedef struct arch_cpu_state arch_cpu_state_t;

#endif /* KERNUL_ARCH_H */
