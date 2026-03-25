#ifndef KERNUL_INIT_H
#define KERNUL_INIT_H

/*
 * include/kernul/init.h
 *
 * Kernel-internal init process launch-path contract. Phase 5 boundary.
 *
 * This surface defines deterministic launch sequencing shape only.
 * It is not a userspace ABI surface and does not make compatibility claims.
 *
 * Non-goals:
 *   - no userspace resume or trap return path
 *   - no loader execution
 *   - no VM mapping, paging, or MMU behavior
 *
 * Calling context:
 *   May be invoked during deterministic bootstrap/runtime transition once
 *   Phase 5 process/address-space/ELF contract surfaces are available.
 *   This contract does not require scheduler state beyond guarantees already
 *   established elsewhere.
 *
 * Determinism:
 *   The launch attempt follows a fixed, reviewable sequence with no hidden
 *   work and no policy decisions.
 */

/*
 * init_launch() - Deterministic init userspace-transition attempt boundary.
 *
 * Sequence shape:
 *   1) obtain process object
 *   2) obtain address-space object and associate structurally
 *   3) obtain ELF image object and associate borrowed links structurally
 *   4) validate image state via elf_image_validate()
 *   5) stop deterministically at this phase by returning fixed KERN_ENOSYS
 *
 * CONTEXT: bootstrap/runtime transition per contract above.
 * SLEEP: implementation-defined (allocation stubs may be used).
 * ALLOC: only via existing process/address-space/ELF alloc surfaces.
 * I/O: no.
 * LOCKS: no lock required by caller in this phase.
 * OWNERSHIP:
 *   - process/address-space/image allocations return owned references
 *   - elf_image.owner and elf_image.address_space are borrowed links
 * FAILURE:
 *   - fixed KERN_ENOSYS for the Phase 5 boundary stop
 *   - KERN_ENOMEM for deterministic allocation failure before boundary stop
 */
int init_launch(void);

#endif /* KERNUL_INIT_H */
