#ifndef ARCH_AARCH64_CPU_STATE_H
#define ARCH_AARCH64_CPU_STATE_H

/*
 * arch/aarch64/cpu_state.h
 *
 * AArch64 definition of arch_cpu_state_t. §2.9.3, Phase 1.
 *
 * Phase 1 scope: placeholder type only.
 *
 * The full register layout (x19-x28, fp, lr, sp, pc, spsr, tpidr),
 * arch_cpu_state_init(), and arch_cpu_state_switch() are Phase 4
 * deliverables. They are not defined here.
 *
 * Note on ordering: when the Phase 4 switch implementation is added,
 * it must include a DSB ISH + ISB to preserve AArch64 weak-ordering
 * correctness across the context switch boundary.
 */

#include <kernul/types.h>

struct arch_cpu_state {
    u64 _placeholder;   /* no fields defined until Phase 4 */
};

#endif /* ARCH_AARCH64_CPU_STATE_H */
