/*
 * tests/console/test_lifecycle_governance_schema_link.c
 *
 * Link-check for lifecycle governance schema symbols and constraints.
 * Pass condition: successful link against governance schema implementation.
 */

#include <kernul/lifecycle_governance_schema.h>

int main(void)
{
    static const uint32_t expected_phases[] = {
        31U, 33U, 34U, 36U, 37U, 38U, 39U, 40U, 41U, 42U, 43U,
    };
    static const char *const allowed_roles[] = {
        "state_flag",
        "state_value",
        "status_code",
        "timestamp",
        "surface_reference",
        "version_reference",
        "buffer_pointer",
        "buffer_capacity",
        "buffer_length",
        "phase_reference",
        "descriptor_text",
    };
    const uint32_t first_phase = 31U;
    const uint32_t last_phase = 43U;
    size_t i;

    if (lifecycle_governance_schema_count
        != (sizeof(expected_phases) / sizeof(expected_phases[0])))
        return 1;

    for (i = 0U; i < lifecycle_governance_schema_count; i++) {
        const lifecycle_governance_schema_entry_t *entry =
            &lifecycle_governance_schema[i];
        size_t j;

        if (entry->phase_number != expected_phases[i])
            return 1;
        if (entry->phase_number < first_phase || entry->phase_number > last_phase)
            return 1;
        if (i > 0U && lifecycle_governance_schema[i - 1U].phase_number >= entry->phase_number)
            return 1;

        if (entry->surface_name == NULL || entry->surface_name[0] == '\0')
            return 1;

        if (entry->fields == NULL || entry->field_count == 0U)
            return 1;

        for (j = 0U; j < entry->field_count; j++) {
            const lifecycle_governance_schema_field_t *field = &entry->fields[j];
            size_t k;
            int role_match = 0;

            if (field->field_name == NULL || field->field_name[0] == '\0')
                return 1;
            if (field->field_type == NULL || field->field_type[0] == '\0')
                return 1;
            if (field->field_role == NULL || field->field_role[0] == '\0')
                return 1;

            for (k = 0U; k < (sizeof(allowed_roles) / sizeof(allowed_roles[0])); k++) {
                if (__builtin_strcmp(field->field_role, allowed_roles[k]) == 0) {
                    role_match = 1;
                    break;
                }
            }

            if (!role_match)
                return 1;
        }
    }

    return 0;
}
