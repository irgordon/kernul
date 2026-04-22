#ifndef KERNUL_LIFECYCLE_GOVERNANCE_COMPATIBILITY_H
#define KERNUL_LIFECYCLE_GOVERNANCE_COMPATIBILITY_H

/*
 * include/kernul/lifecycle_governance_compatibility.h
 *
 * Kernel-internal descriptive, inert governance compatibility surface.
 * Phase 43, Task 1 boundary.
 */

#include <kernul/types.h>

typedef struct lifecycle_governance_compatibility_entry {
    uint32_t phase_number;              /* unique, stable, 31-40 */
    const char *surface_name;           /* informational label only */
    uint32_t compatible_freeze_version; /* normative: copied from LIFECYCLE_FREEZE_VERSION */
    const char *compatibility_notes;    /* informational label only; from closed vocabulary */
} lifecycle_governance_compatibility_entry_t;

extern const lifecycle_governance_compatibility_entry_t lifecycle_governance_compatibility[];
extern const size_t lifecycle_governance_compatibility_count;

#endif /* KERNUL_LIFECYCLE_GOVERNANCE_COMPATIBILITY_H */
