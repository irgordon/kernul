#ifndef KERNUL_LIFECYCLE_GOVERNANCE_MANIFEST_H
#define KERNUL_LIFECYCLE_GOVERNANCE_MANIFEST_H

/*
 * include/kernul/lifecycle_governance_manifest.h
 *
 * Kernel-internal descriptive, inert governance manifest surface.
 * Phase 45, Task 1 boundary.
 */

#include <kernul/types.h>

typedef struct lifecycle_governance_manifest_entry {
    uint32_t phase_number;        /* 41-44 */
    const char *surface_name;     /* informational label only */
    const char *surface_role;     /* informational label only; from closed vocabulary */
} lifecycle_governance_manifest_entry_t;

extern const lifecycle_governance_manifest_entry_t lifecycle_governance_manifest[];
extern const size_t lifecycle_governance_manifest_count;

#endif /* KERNUL_LIFECYCLE_GOVERNANCE_MANIFEST_H */
