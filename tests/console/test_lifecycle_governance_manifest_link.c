/*
 * tests/console/test_lifecycle_governance_manifest_link.c
 *
 * Link-check for lifecycle governance manifest symbols and constraints.
 * Pass condition: successful link against governance manifest implementation.
 */

#include <kernul/lifecycle_governance_manifest.h>

int main(void)
{
    static const uint32_t expected_phases[] = { 41U, 42U, 43U, 44U };
    static const char *const allowed_roles[] = {
        "index",
        "provenance",
        "compatibility",
        "schema",
    };
    size_t i;

    if (lifecycle_governance_manifest_count
        != (sizeof(expected_phases) / sizeof(expected_phases[0])))
        return 1;

    for (i = 0U; i < lifecycle_governance_manifest_count; i++) {
        const lifecycle_governance_manifest_entry_t *entry =
            &lifecycle_governance_manifest[i];
        size_t j;
        int role_match = 0;

        if (entry->phase_number != expected_phases[i])
            return 1;
        if (entry->surface_name == NULL || entry->surface_name[0] == '\0')
            return 1;
        if (entry->surface_role == NULL || entry->surface_role[0] == '\0')
            return 1;
        if (i > 0U && lifecycle_governance_manifest[i - 1U].phase_number >= entry->phase_number)
            return 1;

        for (j = 0U; j < (sizeof(allowed_roles) / sizeof(allowed_roles[0])); j++) {
            if (__builtin_strcmp(entry->surface_role, allowed_roles[j]) == 0) {
                role_match = 1;
                break;
            }
        }

        if (!role_match)
            return 1;
    }

    return 0;
}
