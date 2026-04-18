/*
 * core/lifecycle_governance_index.c
 *
 * Descriptive, inert lifecycle governance surface index.
 */

#include <kernul/lifecycle_governance_index.h>
#include <kernul/lifecycle_freeze_baseline.h>

_Static_assert(LIFECYCLE_FREEZE_VERSION >= 1U,
               "lifecycle governance index requires a defined freeze baseline version");

const lifecycle_governance_index_entry_t lifecycle_governance_index[] = {
    { .surface_name = "Lifecycle introspection surface", .phase_number = 31U },
    { .surface_name = "Lifecycle invariant verification surface", .phase_number = 32U },
    { .surface_name = "Lifecycle provenance surface", .phase_number = 33U },
    { .surface_name = "Lifecycle boundary assertion surface", .phase_number = 34U },
    { .surface_name = "Lifecycle freeze enforcement surface", .phase_number = 35U },
    { .surface_name = "Lifecycle documentation synthesis surface", .phase_number = 36U },
    { .surface_name = "Lifecycle audit surface", .phase_number = 37U },
    { .surface_name = "Lifecycle export serialization surface", .phase_number = 38U },
    { .surface_name = "Lifecycle extension declaration surface", .phase_number = 39U },
    { .surface_name = "Lifecycle governance consolidation surface", .phase_number = 40U },
};

const size_t lifecycle_governance_index_count =
    sizeof(lifecycle_governance_index) / sizeof(lifecycle_governance_index[0]);
