/*
 * tests/console/test_lifecycle_governance_provenance_link.c
 *
 * Link-check for lifecycle governance provenance symbols and constraints.
 * Pass condition: successful link against governance provenance implementation.
 */

#include <kernul/lifecycle_governance_provenance.h>

int main(void)
{
    const uint32_t first_phase = 31U;
    const uint32_t last_phase = 40U;
    const size_t expected_count = (size_t)(last_phase - first_phase + 1U);
    size_t i;

    if (lifecycle_governance_provenance_count != expected_count)
        return 1;

    for (i = 0U; i < lifecycle_governance_provenance_count; i++) {
        const lifecycle_governance_provenance_entry_t *entry =
            &lifecycle_governance_provenance[i];

        if (entry->phase_number != (uint32_t)(first_phase + i))
            return 1;
        if (entry->phase_number < first_phase || entry->phase_number > last_phase)
            return 1;
        if (i > 0U && lifecycle_governance_provenance[i - 1U].phase_number >= entry->phase_number)
            return 1;

        if (entry->surface_name == NULL || entry->surface_name[0] == '\0')
            return 1;
        if (entry->purpose == NULL || entry->purpose[0] == '\0')
            return 1;
        if (entry->origin == NULL || entry->origin[0] == '\0')
            return 1;
    }

    return 0;
}
