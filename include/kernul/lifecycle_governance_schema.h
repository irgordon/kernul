#ifndef KERNUL_LIFECYCLE_GOVERNANCE_SCHEMA_H
#define KERNUL_LIFECYCLE_GOVERNANCE_SCHEMA_H

/*
 * include/kernul/lifecycle_governance_schema.h
 *
 * Kernel-internal descriptive, inert governance schema surface.
 * Phase 44, Task 1 boundary.
 */

#include <kernul/types.h>

typedef struct lifecycle_governance_schema_field {
    const char *field_name;      /* informational label only */
    const char *field_type;      /* informational label only */
    const char *field_role;      /* informational label only; from closed vocabulary */
} lifecycle_governance_schema_field_t;

typedef struct lifecycle_governance_schema_entry {
    uint32_t phase_number;                       /* 31-43 */
    const char *surface_name;                    /* informational label only */
    const lifecycle_governance_schema_field_t *fields;
    size_t field_count;
} lifecycle_governance_schema_entry_t;

extern const lifecycle_governance_schema_entry_t lifecycle_governance_schema[];
extern const size_t lifecycle_governance_schema_count;

#endif /* KERNUL_LIFECYCLE_GOVERNANCE_SCHEMA_H */
