/*
 * tests/console/test_lifecycle_governance_compatibility_link.c
 *
 * Link-check for lifecycle governance compatibility symbols and constraints.
 * Pass condition: successful link against governance compatibility implementation.
 */

#include <kernul/lifecycle_governance_compatibility.h>
#include <kernul/lifecycle_freeze_baseline.h>

int main(void)
{
    const uint32_t first_phase = 31U;
    const uint32_t last_phase = 40U;
    const size_t expected_count = (size_t)(last_phase - first_phase + 1U);
    size_t i;

    if (lifecycle_governance_compatibility_count != expected_count)
        return 1;

    for (i = 0U; i < lifecycle_governance_compatibility_count; i++) {
        const lifecycle_governance_compatibility_entry_t *entry =
            &lifecycle_governance_compatibility[i];

        if (entry->phase_number != (uint32_t)(first_phase + i))
            return 1;
        if (entry->phase_number < first_phase || entry->phase_number > last_phase)
            return 1;
        if (i > 0U
            && lifecycle_governance_compatibility[i - 1U].phase_number >= entry->phase_number)
            return 1;

        if (entry->surface_name == NULL || entry->surface_name[0] == '\0')
            return 1;

        if (entry->compatible_freeze_version != LIFECYCLE_FREEZE_VERSION)
            return 1;

        if (entry->compatibility_notes == NULL || entry->compatibility_notes[0] == '\0')
            return 1;

        if (__builtin_strcmp(entry->compatibility_notes, "governance_phase_surface") != 0
            && __builtin_strcmp(entry->compatibility_notes,
                                "governance_consolidation_surface")
                   != 0)
            return 1;
    }

    return 0;
}
