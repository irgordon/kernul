/*
 * arch/aarch64/arch.c
 *
 * AArch64 architecture boundary surface implementations. §2.9, Phase 1
 *
 * Implements:
 *   arch_halt()       — mask all interrupts and enter WFI loop
 *   arch_early_init() — minimal EL1 bring-up
 *
 * Constraints (Phase 1):
 *   - No MMU enablement.
 *   - No interrupt handler registration.
 *   - No scheduler assumptions.
 *   - No device or platform discovery.
 */

#include <kernul/arch.h>
#include <kernul/compiler.h>

/*
 * arch_halt — AArch64 terminal execution endpoint. §2.9.1
 *
 * Mask all interrupts via DAIF (Debug, SError, IRQ, FIQ) then execute
 * WFI (Wait For Interrupt) in a loop.
 *
 * We loop because:
 *   1. WFI may return spuriously on some implementations.
 *   2. An NMI-equivalent (SError) may wake the CPU even with IRQ/FIQ masked.
 *
 * Masking all four DAIF bits ensures the CPU makes no forward progress.
 */
KERN_NORETURN void arch_halt(void)
{
    __asm__ volatile("msr daifset, #0xf" ::: "memory");
    for (;;) {
        __asm__ volatile("wfi");
    }
}

/*
 * arch_early_init — AArch64 minimal bring-up. §2.9.2
 *
 * Phase 1 scope: establish the minimum EL1 environment for C execution.
 *
 * Current work:
 *   - Set VBAR_EL1 to a minimal vector table stub that halts on any
 *     unexpected exception during early boot.
 *
 * Future expansion (not in Phase 1):
 *   - Configure TCR_EL1 and MAIR_EL1 before enabling the MMU.
 *   - Detect CPU features via ID registers.
 *   - Set up per-CPU TPIDR_EL1 pointer.
 */

/*
 * early_vectors — minimal EL1 vector table.
 *
 * Each entry halts immediately. This prevents a silent triple-fault
 * equivalent during early boot when no real handler is installed.
 *
 * The table must be aligned to 2048 bytes per the ARMv8 architecture.
 */
extern void early_vectors(void);

void arch_early_init(void)
{
    /*
     * Point VBAR_EL1 at the early halt vectors so any unexpected
     * exception during bootstrap produces a deterministic halt
     * rather than an unpredictable branch to address 0.
     *
     * early_vectors is defined in arch/aarch64/entry.S (Phase 1 assembly,
     * not yet generated). For the stub build this is a forward declaration
     * that does not need to resolve.
     */
#if defined(KERNUL_AARCH64_TARGET)
    __asm__ volatile(
        "msr vbar_el1, %0\n"
        "isb\n"
        :
        : "r"(early_vectors)
        : "memory"
    );
#endif
    /* No other Phase 1 work required. See §2.12. */
}
