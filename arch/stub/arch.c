/*
 * arch/stub/arch.c
 *
 * Generic stub architecture implementation of the three mandatory
 * boundary surfaces. §2.9
 *
 * This stub is for build verification and hosted unit testing only.
 * It is not a real architecture port. Do not copy it as one.
 *
 * A real port provides arch/<target>/arch.c with:
 *   - arch_halt(): disables IRQs and executes the architecture halt instruction
 *   - arch_early_init(): sets up GDT/IDT (x86), exception vectors (AArch64), etc.
 */

#include <kernul/arch.h>
#include <kernul/compiler.h>

KERN_NORETURN void arch_halt(void)
{
    /*
     * Stub: no processor halt instruction available in a hosted environment.
     * Infinite loop satisfies the noreturn contract.
     *
     * A real implementation disables interrupts and executes:
     *   x86-64:  cli; hlt (in a loop in case of spurious NMI)
     *   AArch64: msr daifset, #0xf; wfi (in a loop)
     *   RISC-V:  csrci mstatus, 0x8; wfi (in a loop)
     */
    for (;;) {
        /* intentional spin */
    }
}

void arch_early_init(void)
{
    /*
     * Stub: no real hardware to initialize.
     *
     * A real implementation performs the minimum work to make the CPU
     * usable by C code:
     *   x86-64:  load GDT, set up IDT stubs, configure APIC base
     *   AArch64: set VBAR_EL1, configure TCR_EL1, enable MMU
     *   RISC-V:  set up mtvec/stvec, configure PMP if needed
     */
}
