/*
 * tests/console/test_lifecycle_governance_link.c
 *
 * Link-check for lifecycle governance consolidation symbols.
 * Pass condition: successful link against governance implementation.
 */

#include <kernul/lifecycle_governance.h>
#include <kernul/lifecycle_freeze_baseline.h>

static bool
is_nonempty_label(const char *label)
{
    return (label != NULL && label[0] != '\0');
}

int main(void)
{
    if (lifecycle_governance_summary.freeze_version != (uint32_t)LIFECYCLE_FREEZE_VERSION)
        return 1;

    if (!is_nonempty_label(lifecycle_governance_summary.introspection_surface))
        return 1;
    if (!is_nonempty_label(lifecycle_governance_summary.verification_surface))
        return 1;
    if (!is_nonempty_label(lifecycle_governance_summary.provenance_surface))
        return 1;
    if (!is_nonempty_label(lifecycle_governance_summary.assertion_surface))
        return 1;
    if (!is_nonempty_label(lifecycle_governance_summary.documentation_surface))
        return 1;
    if (!is_nonempty_label(lifecycle_governance_summary.audit_surface))
        return 1;
    if (!is_nonempty_label(lifecycle_governance_summary.export_surface))
        return 1;
    if (!is_nonempty_label(lifecycle_governance_summary.extension_surface))
        return 1;

    return 0;
}
