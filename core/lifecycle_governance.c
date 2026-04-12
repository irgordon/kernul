/*
 * core/lifecycle_governance.c
 *
 * Descriptive, inert lifecycle governance consolidation summary.
 */

#include <kernul/lifecycle_governance.h>
#include <kernul/lifecycle_freeze_baseline.h>

const lifecycle_governance_summary_t lifecycle_governance_summary = {
    .freeze_version = LIFECYCLE_FREEZE_VERSION,
    .introspection_surface = "Lifecycle phase map (0-39) introspection surface",
    .verification_surface = "Lifecycle invariants verification surface",
    .provenance_surface = "Lifecycle provenance model surface",
    .assertion_surface = "Lifecycle boundary assertion surface",
    .documentation_surface = "Lifecycle documentation synthesis surface",
    .audit_surface = "Lifecycle audit surface",
    .export_surface = "Lifecycle export serialization surface",
    .extension_surface = "Lifecycle extension declaration surface",
};
