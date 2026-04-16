/*
 * tests/console/test_lifecycle_governance_index_link.c
 *
 * Link-check for lifecycle governance index symbols and constraints.
 * Pass condition: successful link against governance index implementation.
 */

#include <kernul/lifecycle_governance_index.h>

int main(void)
{
    const uint32_t first_phase = 31U;
    const uint32_t last_phase = 40U;
    const size_t expected_count = (size_t)(last_phase - first_phase + 1U);
    size_t i;

    if (lifecycle_governance_index_count != expected_count)
        return 1;

    for (i = 0U; i < lifecycle_governance_index_count; i++) {
        const lifecycle_governance_index_entry_t *entry =
            &lifecycle_governance_index[i];

        if (entry->surface_name == NULL)
            return 1;
        if (entry->surface_name[0] == '\0')
            return 1;

        if (entry->phase_number != (uint32_t)(first_phase + i))
            return 1;

        if (i > 0U && lifecycle_governance_index[i - 1U].phase_number >= entry->phase_number)
            return 1;
    }

    return 0;
}
