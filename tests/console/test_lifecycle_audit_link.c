/*
 * tests/console/test_lifecycle_audit_link.c
 *
 * Link-check for lifecycle audit dependency boundary symbols.
 * Pass condition: successful link against audit implementation.
 */

#include <kernul/lifecycle_audit.h>
#include <kernul/lifecycle_freeze_baseline.h>

int main(void)
{
    lifecycle_snapshot_t snapshot = {0};
    lifecycle_provenance_t provenance = {0};
    lifecycle_audit_report_t report = {0};

    snapshot.ready = true;
    provenance.readiness_timestamp = 1U;

    if (lifecycle_generate_audit_report(&snapshot,
                                        LIFECYCLE_VERIFY_VERIFIED,
                                        &provenance,
                                        LIFECYCLE_ASSERT_SATISFIED,
                                        &report) != LIFECYCLE_AUDIT_OK) {
        return 1;
    }

    if (!report.snapshot.ready)
        return 1;
    if (report.verify_result != LIFECYCLE_VERIFY_VERIFIED)
        return 1;
    if (report.assertion != LIFECYCLE_ASSERT_SATISFIED)
        return 1;
    if (report.freeze_version != LIFECYCLE_FREEZE_VERSION)
        return 1;

    return 0;
}
