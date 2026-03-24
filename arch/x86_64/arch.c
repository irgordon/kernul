/*
 * arch/x86_64/arch.c
 *
 * x86-64 architecture boundary surface implementations. §2.9, Phase 1
 *
 * Implements the three mandatory boundary surfaces for x86-64:
 *   arch_halt()       — disable interrupts and halt the processor
 *   arch_early_init() — minimal CPU bring-up before subsystem init
 *
 * arch_cpu_state is defined in arch/x86_64/cpu_state.h.
 *
 * Constraints (Phase 1):
 *   - No interrupt handling setup.
 *   - No MMU enablement.
 *   - No scheduler assumptions.
 *   - No device initialization.
 *   - No platform discovery.
 */

#include <kernul/arch.h>
#include <kernul/compiler.h>

/*
 * arch_halt — x86-64 terminal execution endpoint. §2.9.1
 *
 * Disable interrupts and execute HLT in a loop.
 * The loop handles the rare case of a non-maskable interrupt (NMI)
 * waking the CPU after HLT; we immediately re-halt.
 *
 * This function must never be called for recoverable conditions.
 */
KERN_NORETURN void arch_halt(void)
{
    __asm__ volatile("cli" ::: "memory");
    for (;;) {
        __asm__ volatile("hlt");
    }
}

/*
 * arch_early_init — x86-64 minimal bring-up. §2.9.2
 *
 * Phase 1 scope: establish the minimum CPU state required for C code
 * to execute correctly. No device discovery, no MMU, no interrupts.
 *
 * Current work:
 *   - None required beyond what the bootloader provides for a flat
 *     protected/long-mode environment.
 *
 * Future expansion (not in Phase 1):
 *   - Load a minimal GDT if not provided by bootloader.
 *   - Set up IDT stubs to prevent triple-fault on unexpected exceptions.
 *   - Detect CPUID feature flags needed by later subsystems.
 */
void arch_early_init(void)
{
    /*
     * Phase 1: no-op on x86-64 when entering from a standard bootloader
     * (e.g., multiboot2, UEFI stub) that has already established a valid
     * long-mode environment.
     *
     * Do not add behavior here without a concrete Phase 1 requirement.
     * See Architecture Guide §2.12 (architectural minimalism rule).
     */
}
