/*
 * core/lifecycle_governance_compatibility.c
 *
 * Descriptive, inert governance compatibility mapping.
 */

#include <kernul/lifecycle_governance_compatibility.h>
#include <kernul/lifecycle_freeze_baseline.h>

#define LIFECYCLE_GOVERNANCE_FIRST_PHASE 31U
#define LIFECYCLE_GOVERNANCE_LAST_PHASE 40U
#define LIFECYCLE_GOVERNANCE_COMPATIBILITY_EXPECTED_COUNT \
    ((size_t)(LIFECYCLE_GOVERNANCE_LAST_PHASE - LIFECYCLE_GOVERNANCE_FIRST_PHASE + 1U))

static const char COMPATIBILITY_NOTE_GOVERNANCE_PHASE_SURFACE[] =
    "governance_phase_surface";
static const char COMPATIBILITY_NOTE_GOVERNANCE_CONSOLIDATION_SURFACE[] =
    "governance_consolidation_surface";

_Static_assert(LIFECYCLE_FREEZE_VERSION >= 1U,
               "governance compatibility requires a defined freeze baseline version");

const lifecycle_governance_compatibility_entry_t lifecycle_governance_compatibility[] = {
    {
        .phase_number = 31U,
        .surface_name = "Lifecycle introspection surface",
        .compatible_freeze_version = LIFECYCLE_FREEZE_VERSION,
        .compatibility_notes = COMPATIBILITY_NOTE_GOVERNANCE_PHASE_SURFACE,
    },
    {
        .phase_number = 32U,
        .surface_name = "Lifecycle invariant verification surface",
        .compatible_freeze_version = LIFECYCLE_FREEZE_VERSION,
        .compatibility_notes = COMPATIBILITY_NOTE_GOVERNANCE_PHASE_SURFACE,
    },
    {
        .phase_number = 33U,
        .surface_name = "Lifecycle provenance surface",
        .compatible_freeze_version = LIFECYCLE_FREEZE_VERSION,
        .compatibility_notes = COMPATIBILITY_NOTE_GOVERNANCE_PHASE_SURFACE,
    },
    {
        .phase_number = 34U,
        .surface_name = "Lifecycle boundary assertion surface",
        .compatible_freeze_version = LIFECYCLE_FREEZE_VERSION,
        .compatibility_notes = COMPATIBILITY_NOTE_GOVERNANCE_PHASE_SURFACE,
    },
    {
        .phase_number = 35U,
        .surface_name = "Lifecycle freeze enforcement surface",
        .compatible_freeze_version = LIFECYCLE_FREEZE_VERSION,
        .compatibility_notes = COMPATIBILITY_NOTE_GOVERNANCE_PHASE_SURFACE,
    },
    {
        .phase_number = 36U,
        .surface_name = "Lifecycle documentation synthesis surface",
        .compatible_freeze_version = LIFECYCLE_FREEZE_VERSION,
        .compatibility_notes = COMPATIBILITY_NOTE_GOVERNANCE_PHASE_SURFACE,
    },
    {
        .phase_number = 37U,
        .surface_name = "Lifecycle audit surface",
        .compatible_freeze_version = LIFECYCLE_FREEZE_VERSION,
        .compatibility_notes = COMPATIBILITY_NOTE_GOVERNANCE_PHASE_SURFACE,
    },
    {
        .phase_number = 38U,
        .surface_name = "Lifecycle export serialization surface",
        .compatible_freeze_version = LIFECYCLE_FREEZE_VERSION,
        .compatibility_notes = COMPATIBILITY_NOTE_GOVERNANCE_PHASE_SURFACE,
    },
    {
        .phase_number = 39U,
        .surface_name = "Lifecycle extension declaration surface",
        .compatible_freeze_version = LIFECYCLE_FREEZE_VERSION,
        .compatibility_notes = COMPATIBILITY_NOTE_GOVERNANCE_PHASE_SURFACE,
    },
    {
        .phase_number = 40U,
        .surface_name = "Lifecycle governance consolidation surface",
        .compatible_freeze_version = LIFECYCLE_FREEZE_VERSION,
        .compatibility_notes = COMPATIBILITY_NOTE_GOVERNANCE_CONSOLIDATION_SURFACE,
    },
};

const size_t lifecycle_governance_compatibility_count =
    sizeof(lifecycle_governance_compatibility) / sizeof(lifecycle_governance_compatibility[0]);

_Static_assert((sizeof(lifecycle_governance_compatibility)
                / sizeof(lifecycle_governance_compatibility[0]))
                   == LIFECYCLE_GOVERNANCE_COMPATIBILITY_EXPECTED_COUNT,
               "governance compatibility count must match canonical phase range 31-40");
