/*
 * core/lifecycle_governance_manifest.c
 *
 * Descriptive, inert governance manifest directory for post-governance
 * metadata surfaces.
 */

#include <kernul/lifecycle_governance_manifest.h>

#define LIFECYCLE_GOVERNANCE_MANIFEST_FIRST_PHASE 41U
#define LIFECYCLE_GOVERNANCE_MANIFEST_LAST_PHASE 44U
#define LIFECYCLE_GOVERNANCE_MANIFEST_EXPECTED_COUNT \
    ((size_t)(LIFECYCLE_GOVERNANCE_MANIFEST_LAST_PHASE - LIFECYCLE_GOVERNANCE_MANIFEST_FIRST_PHASE + 1U))

#define LIFECYCLE_GOVERNANCE_MANIFEST_PHASE_41 LIFECYCLE_GOVERNANCE_MANIFEST_FIRST_PHASE
#define LIFECYCLE_GOVERNANCE_MANIFEST_PHASE_42 (LIFECYCLE_GOVERNANCE_MANIFEST_FIRST_PHASE + 1U)
#define LIFECYCLE_GOVERNANCE_MANIFEST_PHASE_43 (LIFECYCLE_GOVERNANCE_MANIFEST_FIRST_PHASE + 2U)
#define LIFECYCLE_GOVERNANCE_MANIFEST_PHASE_44 LIFECYCLE_GOVERNANCE_MANIFEST_LAST_PHASE

/*
 * Closed, translation-unit-local vocabulary for manifest surface roles.
 * These are informational labels only.
 */
static const char SURFACE_ROLE_INDEX[] = "index";
static const char SURFACE_ROLE_PROVENANCE[] = "provenance";
static const char SURFACE_ROLE_COMPATIBILITY[] = "compatibility";
static const char SURFACE_ROLE_SCHEMA[] = "schema";

static const char *const lifecycle_governance_manifest_surface_role_vocabulary[] = {
    SURFACE_ROLE_INDEX,
    SURFACE_ROLE_PROVENANCE,
    SURFACE_ROLE_COMPATIBILITY,
    SURFACE_ROLE_SCHEMA,
};

#define LIFECYCLE_GOVERNANCE_MANIFEST_ROLE_VOCABULARY_COUNT \
    (sizeof(lifecycle_governance_manifest_surface_role_vocabulary) \
     / sizeof(lifecycle_governance_manifest_surface_role_vocabulary[0]))

const lifecycle_governance_manifest_entry_t lifecycle_governance_manifest[] = {
    {
        .phase_number = LIFECYCLE_GOVERNANCE_MANIFEST_PHASE_41,
        .surface_name = "Governance index surface",
        .surface_role = SURFACE_ROLE_INDEX,
    },
    {
        .phase_number = LIFECYCLE_GOVERNANCE_MANIFEST_PHASE_42,
        .surface_name = "Governance provenance surface",
        .surface_role = SURFACE_ROLE_PROVENANCE,
    },
    {
        .phase_number = LIFECYCLE_GOVERNANCE_MANIFEST_PHASE_43,
        .surface_name = "Governance compatibility surface",
        .surface_role = SURFACE_ROLE_COMPATIBILITY,
    },
    {
        .phase_number = LIFECYCLE_GOVERNANCE_MANIFEST_PHASE_44,
        .surface_name = "Governance schema surface",
        .surface_role = SURFACE_ROLE_SCHEMA,
    },
};

const size_t lifecycle_governance_manifest_count =
    sizeof(lifecycle_governance_manifest) / sizeof(lifecycle_governance_manifest[0]);

_Static_assert((sizeof(lifecycle_governance_manifest) / sizeof(lifecycle_governance_manifest[0]))
                   == LIFECYCLE_GOVERNANCE_MANIFEST_EXPECTED_COUNT,
               "governance manifest count must match canonical phase range 41-44");

/*
 * The role vocabulary is closed and translation-unit-local. Its size is
 * asserted independently from the manifest count so the vocabulary remains
 * fixed without implying that vocabulary cardinality must equal manifest
 * cardinality as a semantic rule.
 */
_Static_assert(LIFECYCLE_GOVERNANCE_MANIFEST_ROLE_VOCABULARY_COUNT == 4U,
               "governance manifest role vocabulary must contain exactly four literals");
