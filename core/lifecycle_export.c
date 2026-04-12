/*
 * core/lifecycle_export.c
 *
 * Deterministic, order-stable lifecycle audit export serialization.
 */

#include <kernul/lifecycle_export.h>
#include <kernul/lifecycle_audit.h>
#include <kernul/lifecycle_freeze_baseline.h>

#define LIFECYCLE_EXPORT_MAX_U64_DECIMAL_DIGITS 20U

typedef struct lifecycle_export_writer {
    /* buffer may be NULL for dry-run sizing; capacity bounds are still enforced */
    char *buffer;
    size_t capacity;
    size_t length;
} lifecycle_export_writer_t;

static bool
writer_append_char(lifecycle_export_writer_t *writer, char ch)
{
    if (writer->length == SIZE_MAX)
        return false;
    if (writer->length == writer->capacity)
        return false;

    if (writer->buffer != NULL)
        writer->buffer[writer->length] = ch;
    writer->length++;
    return true;
}

static bool
writer_append_cstr(lifecycle_export_writer_t *writer, const char *text)
{
    size_t i;

    if (text == NULL)
        return false;

    for (i = 0U; text[i] != '\0'; i++) {
        if (!writer_append_char(writer, text[i]))
            return false;
    }

    return true;
}

static bool
writer_append_u64(lifecycle_export_writer_t *writer, u64 value)
{
    char digits[LIFECYCLE_EXPORT_MAX_U64_DECIMAL_DIGITS];
    size_t digit_count;
    u64 work;

    digit_count = 0U;
    work = value;

    do {
        digits[digit_count] = (char)('0' + (work % 10U));
        digit_count++;
        work /= 10U;
    } while (work != 0U);

    while (digit_count > 0U) {
        digit_count--;
        if (!writer_append_char(writer, digits[digit_count]))
            return false;
    }

    return true;
}

static u64
s64_to_magnitude(s64 value)
{
    if (value >= 0)
        return (u64)value;

    /*
     * Compute absolute value without signed overflow when value is the
     * minimum representable s64.
     */
    return (u64)(-(value + 1)) + 1U;
}

static bool
writer_append_s64(lifecycle_export_writer_t *writer, s64 value)
{
    if (value < 0) {
        if (!writer_append_char(writer, '-'))
            return false;
        return writer_append_u64(writer, s64_to_magnitude(value));
    }

    return writer_append_u64(writer, s64_to_magnitude(value));
}

static bool
writer_append_line_u64(
    lifecycle_export_writer_t *writer,
    const char *key,
    u64 value)
{
    if (!writer_append_cstr(writer, key))
        return false;
    if (!writer_append_cstr(writer, ": "))
        return false;
    if (!writer_append_u64(writer, value))
        return false;
    if (!writer_append_char(writer, '\n'))
        return false;

    return true;
}

static bool
writer_append_line_s64(
    lifecycle_export_writer_t *writer,
    const char *key,
    s64 value)
{
    if (!writer_append_cstr(writer, key))
        return false;
    if (!writer_append_cstr(writer, ": "))
        return false;
    if (!writer_append_s64(writer, value))
        return false;
    if (!writer_append_char(writer, '\n'))
        return false;

    return true;
}

static bool
lifecycle_export_structurally_usable(void)
{
    if (LIFECYCLE_FREEZE_VERSION == 0U)
        return false;
    if (sizeof(lifecycle_snapshot_t) != LIFECYCLE_STATE_SIZE)
        return false;
    if (offsetof(lifecycle_snapshot_t, finalized) != LIFECYCLE_STATE_FINALIZED_OFFSET)
        return false;
    if (offsetof(lifecycle_snapshot_t, ready) != LIFECYCLE_STATE_READY_OFFSET)
        return false;
    if (offsetof(lifecycle_snapshot_t, recovery_outcome) != RECOVERY_OUTCOME_OFFSET)
        return false;
    if (offsetof(lifecycle_snapshot_t, retry_outcome) != RETRY_OUTCOME_OFFSET)
        return false;
    if (sizeof(lifecycle_provenance_t) != LIFECYCLE_PROVENANCE_SIZE)
        return false;
    if (offsetof(lifecycle_provenance_t, recovery_outcome_timestamp)
        != LIFECYCLE_PROVENANCE_RECOVERY_OUTCOME_TIMESTAMP_OFFSET) {
        return false;
    }
    if (offsetof(lifecycle_provenance_t, retry_outcome_timestamp)
        != LIFECYCLE_PROVENANCE_RETRY_OUTCOME_TIMESTAMP_OFFSET) {
        return false;
    }

    return true;
}

static bool
lifecycle_export_render(
    lifecycle_export_writer_t *writer,
    const lifecycle_audit_report_t *report)
{
    if (writer == NULL)
        return false;
    if (report == NULL)
        return false;

    if (!writer_append_line_u64(writer, "freeze_version", (u64)report->freeze_version))
        return false;

    if (!writer_append_line_u64(writer, "snapshot.finalized", report->snapshot.finalized ? 1U : 0U))
        return false;
    if (!writer_append_line_u64(writer, "snapshot.ready", report->snapshot.ready ? 1U : 0U))
        return false;
    if (!writer_append_line_u64(writer, "snapshot.owner_id", report->snapshot.owner_id))
        return false;
    if (!writer_append_line_u64(writer, "snapshot.reclaimed", report->snapshot.reclaimed ? 1U : 0U))
        return false;
    if (!writer_append_line_u64(writer,
                                "snapshot.recovery_eligible",
                                report->snapshot.recovery_eligible ? 1U : 0U)) {
        return false;
    }
    if (!writer_append_line_u64(writer,
                                "snapshot.recovery_authorized",
                                report->snapshot.recovery_authorized ? 1U : 0U)) {
        return false;
    }
    if (!writer_append_line_u64(writer,
                                "snapshot.recovery_executed",
                                report->snapshot.recovery_executed ? 1U : 0U)) {
        return false;
    }
    if (!writer_append_line_s64(writer,
                                "snapshot.recovery_execution_result",
                                (s64)report->snapshot.recovery_execution_result)) {
        return false;
    }
    if (!writer_append_line_s64(writer, "snapshot.recovery_outcome", (s64)report->snapshot.recovery_outcome))
        return false;
    if (!writer_append_line_s64(writer, "snapshot.retry_policy", (s64)report->snapshot.retry_policy))
        return false;
    if (!writer_append_line_u64(writer,
                                "snapshot.retry_authorized",
                                report->snapshot.retry_authorized ? 1U : 0U)) {
        return false;
    }
    if (!writer_append_line_u64(writer,
                                "snapshot.retry_executed",
                                report->snapshot.retry_executed ? 1U : 0U)) {
        return false;
    }
    if (!writer_append_line_s64(writer,
                                "snapshot.retry_execution_result",
                                (s64)report->snapshot.retry_execution_result)) {
        return false;
    }
    if (!writer_append_line_s64(writer, "snapshot.retry_outcome", (s64)report->snapshot.retry_outcome))
        return false;

    if (!writer_append_line_s64(writer, "verification_result", (s64)report->verify_result))
        return false;

    if (!writer_append_line_u64(writer,
                                "provenance.finalization_timestamp",
                                report->provenance.finalization_timestamp)) {
        return false;
    }
    if (!writer_append_line_u64(writer,
                                "provenance.readiness_timestamp",
                                report->provenance.readiness_timestamp)) {
        return false;
    }
    if (!writer_append_line_u64(writer,
                                "provenance.ownership_timestamp",
                                report->provenance.ownership_timestamp)) {
        return false;
    }
    if (!writer_append_line_u64(writer,
                                "provenance.reclamation_timestamp",
                                report->provenance.reclamation_timestamp)) {
        return false;
    }
    if (!writer_append_line_u64(writer,
                                "provenance.recovery_eligibility_timestamp",
                                report->provenance.recovery_eligibility_timestamp)) {
        return false;
    }
    if (!writer_append_line_u64(writer,
                                "provenance.recovery_authorization_timestamp",
                                report->provenance.recovery_authorization_timestamp)) {
        return false;
    }
    if (!writer_append_line_u64(writer,
                                "provenance.recovery_execution_timestamp",
                                report->provenance.recovery_execution_timestamp)) {
        return false;
    }
    if (!writer_append_line_u64(writer,
                                "provenance.recovery_outcome_timestamp",
                                report->provenance.recovery_outcome_timestamp)) {
        return false;
    }
    if (!writer_append_line_u64(writer,
                                "provenance.retry_policy_timestamp",
                                report->provenance.retry_policy_timestamp)) {
        return false;
    }
    if (!writer_append_line_u64(writer,
                                "provenance.retry_authorization_timestamp",
                                report->provenance.retry_authorization_timestamp)) {
        return false;
    }
    if (!writer_append_line_u64(writer,
                                "provenance.retry_execution_timestamp",
                                report->provenance.retry_execution_timestamp)) {
        return false;
    }
    if (!writer_append_line_u64(writer,
                                "provenance.retry_outcome_timestamp",
                                report->provenance.retry_outcome_timestamp)) {
        return false;
    }

    if (!writer_append_line_s64(writer, "assertion_result", (s64)report->assertion))
        return false;

    return true;
}

lifecycle_export_result_t
lifecycle_serialize_audit_report(
    const lifecycle_audit_report_t *report,
    lifecycle_export_buffer_t *out_export)
{
    lifecycle_export_writer_t dry_run;
    lifecycle_export_writer_t write_run;

    if (out_export != NULL)
        out_export->length = 0U;

    if (report == NULL)
        return LIFECYCLE_EXPORT_FAILED;
    if (out_export == NULL)
        return LIFECYCLE_EXPORT_FAILED;
    if (out_export->buffer == NULL)
        return LIFECYCLE_EXPORT_FAILED;
    if (out_export->capacity == 0U)
        return LIFECYCLE_EXPORT_FAILED;
    if (report->freeze_version != (u32)LIFECYCLE_FREEZE_VERSION)
        return LIFECYCLE_EXPORT_FAILED;
    if (!lifecycle_export_structurally_usable())
        return LIFECYCLE_EXPORT_FAILED;

    dry_run.buffer = NULL;
    dry_run.capacity = SIZE_MAX;
    dry_run.length = 0U;
    if (!lifecycle_export_render(&dry_run, report))
        return LIFECYCLE_EXPORT_FAILED;
    if (dry_run.length > out_export->capacity)
        return LIFECYCLE_EXPORT_FAILED;

    write_run.buffer = out_export->buffer;
    write_run.capacity = out_export->capacity;
    write_run.length = 0U;
    if (!lifecycle_export_render(&write_run, report))
        return LIFECYCLE_EXPORT_FAILED;

    out_export->length = write_run.length;
    return LIFECYCLE_EXPORT_OK;
}
