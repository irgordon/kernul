/*
 * tests/console/test_lifecycle_extension_link.c
 *
 * Link-check for lifecycle extension declaration symbols.
 * Pass condition: successful link against declaration implementation.
 */

#include <kernul/lifecycle_extension.h>
#include <kernul/lifecycle_freeze_baseline.h>

static bool
is_upper_digit_underscore(char ch)
{
    if (ch >= 'A' && ch <= 'Z')
        return true;
    if (ch >= '0' && ch <= '9')
        return true;
    if (ch == '_')
        return true;
    return false;
}

static bool
is_id_equal(const char *a, const char *b)
{
    size_t i;

    if (a == NULL || b == NULL)
        return false;

    for (i = 0U; ; i++) {
        if (a[i] != b[i])
            return false;
        if (a[i] == '\0')
            return true;
    }
}

int main(void)
{
    size_t i;
    size_t j;

    if (lifecycle_extension_declarations_count == 0U)
        return 1;

    for (i = 0U; i < lifecycle_extension_declarations_count; i++) {
        const lifecycle_extension_decl_t *decl =
            &lifecycle_extension_declarations[i];
        size_t k;

        if (decl->id == NULL)
            return 1;
        if (decl->id[0] == '\0')
            return 1;
        for (k = 0U; decl->id[k] != '\0'; k++) {
            if (!is_upper_digit_underscore(decl->id[k]))
                return 1;
        }
        if (decl->description == NULL)
            return 1;
        if (decl->target_freeze_version < (uint32_t)LIFECYCLE_FREEZE_VERSION)
            return 1;

        for (j = i + 1U; j < lifecycle_extension_declarations_count; j++) {
            if (is_id_equal(decl->id, lifecycle_extension_declarations[j].id))
                return 1;
        }
    }

    return 0;
}
