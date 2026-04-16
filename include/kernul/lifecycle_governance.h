#ifndef KERNUL_LIFECYCLE_GOVERNANCE_H
#define KERNUL_LIFECYCLE_GOVERNANCE_H

/*
 * include/kernul/lifecycle_governance.h
 *
 * Kernel-internal descriptive, inert lifecycle governance consolidation.
 * Phase 40, Task 1 boundary.
 */

#include <kernul/types.h>

typedef struct lifecycle_governance_summary {
    uint32_t freeze_version;              /* copied directly from LIFECYCLE_FREEZE_VERSION */

    const char *introspection_surface;    /* informational label only */
    const char *verification_surface;     /* informational label only */
    const char *provenance_surface;       /* informational label only */
    const char *assertion_surface;        /* informational label only */

    const char *documentation_surface;    /* informational label only */
    const char *audit_surface;            /* informational label only */
    const char *export_surface;           /* informational label only */
    const char *extension_surface;        /* informational label only */
} lifecycle_governance_summary_t;

extern const lifecycle_governance_summary_t lifecycle_governance_summary;

#endif /* KERNUL_LIFECYCLE_GOVERNANCE_H */
