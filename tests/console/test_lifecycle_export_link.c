/*
 * tests/console/test_lifecycle_export_link.c
 *
 * Link-check for lifecycle export serialization symbols.
 * Pass condition: successful link against export implementation.
 */

#include <kernul/lifecycle_export.h>
#include <kernul/lifecycle_freeze_baseline.h>

#define LIFECYCLE_EXPORT_TEST_BUFFER_SIZE 4096U

int main(void)
{
    size_t i;
    char output_a[LIFECYCLE_EXPORT_TEST_BUFFER_SIZE];
    char output_b[LIFECYCLE_EXPORT_TEST_BUFFER_SIZE];
    lifecycle_audit_report_t report = {0};
    lifecycle_export_buffer_t export_a = {
        .buffer = output_a,
        .capacity = sizeof(output_a),
        .length = 0U,
    };
    lifecycle_export_buffer_t export_b = {
        .buffer = output_b,
        .capacity = sizeof(output_b),
        .length = 0U,
    };

    report.freeze_version = LIFECYCLE_FREEZE_VERSION;
    report.snapshot.finalized = true;
    report.snapshot.ready = true;
    report.snapshot.owner_id = 9U;
    report.snapshot.recovery_executed = true;
    report.snapshot.recovery_execution_result = -7;
    report.snapshot.recovery_outcome = 3;
    report.snapshot.retry_authorized = true;
    report.snapshot.retry_executed = true;
    report.snapshot.retry_execution_result = -11;
    report.snapshot.retry_outcome = 5;
    report.verify_result = LIFECYCLE_VERIFY_VERIFIED;
    report.provenance.readiness_timestamp = 1U;
    report.provenance.recovery_execution_timestamp = 11U;
    report.provenance.retry_execution_timestamp = 13U;
    report.assertion = LIFECYCLE_ASSERT_SATISFIED;

    if (lifecycle_serialize_audit_report(&report, &export_a) != LIFECYCLE_EXPORT_OK)
        return 1;
    if (export_a.length == 0U)
        return 1;
    if (output_a[0] != 'f')
        return 1;

    if (lifecycle_serialize_audit_report(&report, &export_b) != LIFECYCLE_EXPORT_OK)
        return 1;
    if (export_b.length != export_a.length)
        return 1;
    for (i = 0U; i < export_a.length; i++) {
        if (output_a[i] != output_b[i])
            return 1;
    }

    report.freeze_version = LIFECYCLE_FREEZE_VERSION + 1U;
    export_b.length = 123U;
    if (lifecycle_serialize_audit_report(&report, &export_b) != LIFECYCLE_EXPORT_FAILED)
        return 1;
    if (export_b.length != 0U)
        return 1;

    return 0;
}
