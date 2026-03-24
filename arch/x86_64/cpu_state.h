#ifndef ARCH_X86_64_CPU_STATE_H
#define ARCH_X86_64_CPU_STATE_H

/*
 * arch/x86_64/cpu_state.h
 *
 * x86-64 definition of arch_cpu_state_t. §2.9.3, Phase 1.
 *
 * Phase 1 scope: placeholder type only.
 *
 * The full register layout (rsp, rbp, rbx, r12-r15, rip, fs_base),
 * arch_cpu_state_init(), and arch_cpu_state_switch() are Phase 4
 * deliverables. They are not defined here.
 *
 * The struct is defined as opaque to common code. Its shape is
 * architecture-owned and will be filled in during Phase 4.
 */

#include <kernul/types.h>

struct arch_cpu_state {
    u64 _placeholder;   /* no fields defined until Phase 4 */
};

#endif /* ARCH_X86_64_CPU_STATE_H */
