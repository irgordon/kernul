#ifndef KERNUL_LIFECYCLE_GOVERNANCE_PROVENANCE_H
#define KERNUL_LIFECYCLE_GOVERNANCE_PROVENANCE_H

/*
 * include/kernul/lifecycle_governance_provenance.h
 *
 * Kernel-internal descriptive, inert governance provenance surface.
 * Phase 42, Task 1 boundary.
 */

#include <kernul/types.h>

typedef struct lifecycle_governance_provenance_entry {
    uint32_t phase_number;          /* unique, stable, 31-40 */
    const char *surface_name;       /* informational label only */
    const char *purpose;            /* informational label only */
    const char *origin;             /* informational label only; from fixed vocabulary */
} lifecycle_governance_provenance_entry_t;

extern const lifecycle_governance_provenance_entry_t lifecycle_governance_provenance[];
extern const size_t lifecycle_governance_provenance_count;

#endif /* KERNUL_LIFECYCLE_GOVERNANCE_PROVENANCE_H */
