/*
 * core/lifecycle_extension.c
 *
 * Inert, canonical lifecycle extension declaration registry.
 */

#include <kernul/lifecycle_extension.h>
#include <kernul/lifecycle_freeze_baseline.h>

const lifecycle_extension_decl_t lifecycle_extension_declarations[] = {
    {
        .id = "LIFECYCLE_EXTENSION_AUDIT_SCHEMA_V2",
        .target_freeze_version = LIFECYCLE_FREEZE_VERSION + 1U,
        .description = "Informational declaration for potential future audit export schema evolution.",
    },
    {
        .id = "LIFECYCLE_EXTENSION_DIAGNOSTIC_ENVELOPE_V2",
        .target_freeze_version = LIFECYCLE_FREEZE_VERSION + 1U,
        .description = "Informational declaration for potential future lifecycle diagnostic envelope shape.",
    },
    {
        .id = "LIFECYCLE_EXTENSION_PHASEMAP_ALIGNMENT_V2",
        .target_freeze_version = LIFECYCLE_FREEZE_VERSION + 2U,
        .description = "Informational declaration for potential future lifecycle phase map alignment metadata.",
    },
};

const size_t lifecycle_extension_declarations_count =
    sizeof(lifecycle_extension_declarations) / sizeof(lifecycle_extension_declarations[0]);
