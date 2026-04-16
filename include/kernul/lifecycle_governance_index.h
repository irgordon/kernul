#ifndef KERNUL_LIFECYCLE_GOVERNANCE_INDEX_H
#define KERNUL_LIFECYCLE_GOVERNANCE_INDEX_H

/*
 * include/kernul/lifecycle_governance_index.h
 *
 * Kernel-internal descriptive, inert governance index surface.
 * Phase 41, Task 1 boundary.
 */

#include <kernul/types.h>

typedef struct lifecycle_governance_index_entry {
    const char *surface_name;   /* informational label only */
    uint32_t phase_number;      /* unique, stable, 31-40 */
} lifecycle_governance_index_entry_t;

extern const lifecycle_governance_index_entry_t lifecycle_governance_index[];
extern const size_t lifecycle_governance_index_count;

#endif /* KERNUL_LIFECYCLE_GOVERNANCE_INDEX_H */
