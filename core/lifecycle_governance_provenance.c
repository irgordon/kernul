/*
 * core/lifecycle_governance_provenance.c
 *
 * Descriptive, inert governance provenance map for governance surfaces.
 */

#include <kernul/lifecycle_governance_provenance.h>
#include <kernul/lifecycle_freeze_baseline.h>

#define LIFECYCLE_GOVERNANCE_FIRST_PHASE 31U
#define LIFECYCLE_GOVERNANCE_LAST_PHASE 40U
#define LIFECYCLE_GOVERNANCE_PROVENANCE_EXPECTED_COUNT \
    ((size_t)(LIFECYCLE_GOVERNANCE_LAST_PHASE - LIFECYCLE_GOVERNANCE_FIRST_PHASE + 1U))

static const char ORIGIN_GOVERNANCE_PHASE_DECLARATION[] =
    "governance_phase_surface";
static const char ORIGIN_GOVERNANCE_CONSOLIDATION[] =
    "governance_consolidation_surface";

_Static_assert(LIFECYCLE_FREEZE_VERSION >= 1U,
               "governance provenance requires a defined freeze baseline version");

const lifecycle_governance_provenance_entry_t lifecycle_governance_provenance[] = {
    {
        .phase_number = 31U,
        .surface_name = "Lifecycle introspection surface",
        .purpose = "Read-only lifecycle introspection.",
        .origin = ORIGIN_GOVERNANCE_PHASE_DECLARATION,
    },
    {
        .phase_number = 32U,
        .surface_name = "Lifecycle invariant verification surface",
        .purpose = "Passive lifecycle invariant verification.",
        .origin = ORIGIN_GOVERNANCE_PHASE_DECLARATION,
    },
    {
        .phase_number = 33U,
        .surface_name = "Lifecycle provenance surface",
        .purpose = "Read-only lifecycle provenance capture.",
        .origin = ORIGIN_GOVERNANCE_PHASE_DECLARATION,
    },
    {
        .phase_number = 34U,
        .surface_name = "Lifecycle boundary assertion surface",
        .purpose = "Declarative lifecycle boundary assertions.",
        .origin = ORIGIN_GOVERNANCE_PHASE_DECLARATION,
    },
    {
        .phase_number = 35U,
        .surface_name = "Lifecycle freeze enforcement surface",
        .purpose = "Compile-time lifecycle freeze enforcement.",
        .origin = ORIGIN_GOVERNANCE_PHASE_DECLARATION,
    },
    {
        .phase_number = 36U,
        .surface_name = "Lifecycle documentation synthesis surface",
        .purpose = "Deterministic lifecycle documentation synthesis.",
        .origin = ORIGIN_GOVERNANCE_PHASE_DECLARATION,
    },
    {
        .phase_number = 37U,
        .surface_name = "Lifecycle audit surface",
        .purpose = "Deterministic lifecycle audit reporting.",
        .origin = ORIGIN_GOVERNANCE_PHASE_DECLARATION,
    },
    {
        .phase_number = 38U,
        .surface_name = "Lifecycle export serialization surface",
        .purpose = "Deterministic lifecycle export serialization.",
        .origin = ORIGIN_GOVERNANCE_PHASE_DECLARATION,
    },
    {
        .phase_number = 39U,
        .surface_name = "Lifecycle extension declaration surface",
        .purpose = "Inert lifecycle extension declarations.",
        .origin = ORIGIN_GOVERNANCE_PHASE_DECLARATION,
    },
    {
        .phase_number = 40U,
        .surface_name = "Lifecycle governance consolidation surface",
        .purpose = "Descriptive governance surface consolidation.",
        .origin = ORIGIN_GOVERNANCE_CONSOLIDATION,
    },
};

const size_t lifecycle_governance_provenance_count =
    sizeof(lifecycle_governance_provenance) / sizeof(lifecycle_governance_provenance[0]);

_Static_assert((sizeof(lifecycle_governance_provenance)
                / sizeof(lifecycle_governance_provenance[0]))
                   == LIFECYCLE_GOVERNANCE_PROVENANCE_EXPECTED_COUNT,
               "governance provenance count must match canonical phase range 31-40");
