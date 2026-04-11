/*
 * core/lifecycle_documentation.c
 *
 * Deterministic, read-only lifecycle documentation synthesis.
 */

#include <stddef.h>

#include <kernul/lifecycle_documentation.h>
#include <kernul/lifecycle_introspection.h>
#include <kernul/lifecycle_verification.h>
#include <kernul/lifecycle_provenance.h>
#include <kernul/lifecycle_assertion.h>
#include <kernul/lifecycle_freeze_baseline.h>

#define LIFECYCLE_DOC_PHASE_FIRST 0U
#define LIFECYCLE_DOC_PHASE_LAST  35U

typedef struct lifecycle_doc_writer {
    char *buffer;
    size_t capacity;
    size_t length;
} lifecycle_doc_writer_t;

static bool
writer_append_char(lifecycle_doc_writer_t *writer, char ch)
{
    if (writer->length == writer->capacity)
        return false;

    if (writer->buffer != NULL)
        writer->buffer[writer->length] = ch;
    writer->length++;
    return true;
}

static bool
writer_append_cstr(lifecycle_doc_writer_t *writer, const char *text)
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
writer_append_u64(lifecycle_doc_writer_t *writer, u64 value)
{
    char digits[32];
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

static bool
writer_append_line_u64(lifecycle_doc_writer_t *writer, const char *key, u64 value)
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
lifecycle_documentation_required_inputs_present(void)
{
    if (LIFECYCLE_DOC_PHASE_FIRST > LIFECYCLE_DOC_PHASE_LAST)
        return false;
    if (LIFECYCLE_FREEZE_VERSION == 0U)
        return false;
    if (LIFECYCLE_STATE_SIZE == 0U)
        return false;
    if (LIFECYCLE_PROVENANCE_SIZE == 0U)
        return false;
    if (sizeof(lifecycle_snapshot_t) == 0U)
        return false;
    if (sizeof(lifecycle_provenance_t) == 0U)
        return false;
    if (sizeof(lifecycle_boundary_t) == 0U)
        return false;
    if (sizeof(lifecycle_documentation_t) == 0U)
        return false;

    return true;
}

static bool
lifecycle_documentation_render(lifecycle_doc_writer_t *writer)
{
    u64 phase;

    if (writer == NULL)
        return false;

    if (!writer_append_cstr(writer, "1. Freeze version\n"))
        return false;
    if (!writer_append_line_u64(writer, "LIFECYCLE_FREEZE_VERSION",
                                (u64)LIFECYCLE_FREEZE_VERSION)) {
        return false;
    }
    if (!writer_append_char(writer, '\n'))
        return false;

    if (!writer_append_cstr(writer, "2. Lifecycle phase map\n"))
        return false;
    for (phase = (u64)LIFECYCLE_DOC_PHASE_FIRST; phase <= (u64)LIFECYCLE_DOC_PHASE_LAST; phase++) {
        if (!writer_append_cstr(writer, "phase: "))
            return false;
        if (!writer_append_u64(writer, phase))
            return false;
        if (!writer_append_char(writer, '\n'))
            return false;
    }
    if (!writer_append_char(writer, '\n'))
        return false;

    if (!writer_append_cstr(writer, "3. Lifecycle invariants\n"))
        return false;
    if (!writer_append_line_u64(writer, "LIFECYCLE_VERIFY_VERIFIED",
                                (u64)LIFECYCLE_VERIFY_VERIFIED)) {
        return false;
    }
    if (!writer_append_line_u64(writer, "LIFECYCLE_VERIFY_VIOLATION",
                                (u64)LIFECYCLE_VERIFY_VIOLATION)) {
        return false;
    }
    if (!writer_append_line_u64(writer, "LIFECYCLE_VERIFY_UNKNOWN",
                                (u64)LIFECYCLE_VERIFY_UNKNOWN)) {
        return false;
    }
    if (!writer_append_cstr(writer,
                            "invariant: ready_implies_finalized\n"
                            "invariant: recovery_execution_requires_authorization\n"
                            "invariant: recovery_outcome_requires_execution\n"
                            "invariant: retry_authorization_requires_allowed_policy\n"
                            "invariant: retry_execution_requires_authorization\n"
                            "invariant: retry_outcome_requires_execution_result\n")) {
        return false;
    }
    if (!writer_append_char(writer, '\n'))
        return false;

    if (!writer_append_cstr(writer, "4. Provenance model\n"))
        return false;
    if (!writer_append_line_u64(writer, "LIFECYCLE_PROVENANCE_OK",
                                (u64)LIFECYCLE_PROVENANCE_OK)) {
        return false;
    }
    if (!writer_append_line_u64(writer, "LIFECYCLE_PROVENANCE_NOT_READY",
                                (u64)LIFECYCLE_PROVENANCE_NOT_READY)) {
        return false;
    }
    if (!writer_append_line_u64(writer, "LIFECYCLE_PROVENANCE_FAILED",
                                (u64)LIFECYCLE_PROVENANCE_FAILED)) {
        return false;
    }
    if (!writer_append_line_u64(writer, "sizeof(lifecycle_provenance_t)",
                                (u64)sizeof(lifecycle_provenance_t))) {
        return false;
    }
    if (!writer_append_line_u64(
            writer,
            "offset(recovery_outcome_timestamp)",
            (u64)offsetof(lifecycle_provenance_t, recovery_outcome_timestamp))) {
        return false;
    }
    if (!writer_append_line_u64(
            writer,
            "offset(retry_outcome_timestamp)",
            (u64)offsetof(lifecycle_provenance_t, retry_outcome_timestamp))) {
        return false;
    }
    if (!writer_append_char(writer, '\n'))
        return false;

    if (!writer_append_cstr(writer, "5. Assertion model\n"))
        return false;
    if (!writer_append_line_u64(writer, "LIFECYCLE_ASSERT_SATISFIED",
                                (u64)LIFECYCLE_ASSERT_SATISFIED)) {
        return false;
    }
    if (!writer_append_line_u64(writer, "LIFECYCLE_ASSERT_VIOLATED",
                                (u64)LIFECYCLE_ASSERT_VIOLATED)) {
        return false;
    }
    if (!writer_append_line_u64(writer, "LIFECYCLE_ASSERT_UNKNOWN",
                                (u64)LIFECYCLE_ASSERT_UNKNOWN)) {
        return false;
    }
    if (!writer_append_line_u64(writer, "sizeof(lifecycle_boundary_t)",
                                (u64)sizeof(lifecycle_boundary_t))) {
        return false;
    }
    if (!writer_append_line_u64(writer, "offset(require_finalized)",
                                (u64)offsetof(lifecycle_boundary_t, require_finalized))) {
        return false;
    }
    if (!writer_append_line_u64(writer, "offset(require_retry_complete)",
                                (u64)offsetof(lifecycle_boundary_t, require_retry_complete))) {
        return false;
    }
    if (!writer_append_char(writer, '\n'))
        return false;

    if (!writer_append_cstr(writer, "6. Lifecycle type definitions\n"))
        return false;
    if (!writer_append_line_u64(writer, "sizeof(lifecycle_snapshot_t)",
                                (u64)sizeof(lifecycle_snapshot_t))) {
        return false;
    }
    if (!writer_append_line_u64(writer, "sizeof(lifecycle_provenance_t)",
                                (u64)sizeof(lifecycle_provenance_t))) {
        return false;
    }
    if (!writer_append_line_u64(writer, "sizeof(lifecycle_boundary_t)",
                                (u64)sizeof(lifecycle_boundary_t))) {
        return false;
    }
    if (!writer_append_line_u64(writer, "sizeof(lifecycle_documentation_t)",
                                (u64)sizeof(lifecycle_documentation_t))) {
        return false;
    }
    if (!writer_append_char(writer, '\n'))
        return false;

    if (!writer_append_cstr(writer,
                            "7. Freeze baseline (sizes, offsets, signatures)\n")) {
        return false;
    }
    if (!writer_append_line_u64(writer, "LIFECYCLE_STATE_SIZE",
                                (u64)LIFECYCLE_STATE_SIZE)) {
        return false;
    }
    if (!writer_append_line_u64(writer, "LIFECYCLE_STATE_FINALIZED_OFFSET",
                                (u64)LIFECYCLE_STATE_FINALIZED_OFFSET)) {
        return false;
    }
    if (!writer_append_line_u64(writer, "LIFECYCLE_STATE_READY_OFFSET",
                                (u64)LIFECYCLE_STATE_READY_OFFSET)) {
        return false;
    }
    if (!writer_append_line_u64(writer, "RECOVERY_OUTCOME_OFFSET",
                                (u64)RECOVERY_OUTCOME_OFFSET)) {
        return false;
    }
    if (!writer_append_line_u64(writer, "RETRY_OUTCOME_OFFSET",
                                (u64)RETRY_OUTCOME_OFFSET)) {
        return false;
    }
    if (!writer_append_line_u64(writer, "LIFECYCLE_PROVENANCE_SIZE",
                                (u64)LIFECYCLE_PROVENANCE_SIZE)) {
        return false;
    }
    if (!writer_append_line_u64(
            writer,
            "LIFECYCLE_PROVENANCE_RECOVERY_OUTCOME_TIMESTAMP_OFFSET",
            (u64)LIFECYCLE_PROVENANCE_RECOVERY_OUTCOME_TIMESTAMP_OFFSET)) {
        return false;
    }
    if (!writer_append_line_u64(
            writer,
            "LIFECYCLE_PROVENANCE_RETRY_OUTCOME_TIMESTAMP_OFFSET",
            (u64)LIFECYCLE_PROVENANCE_RETRY_OUTCOME_TIMESTAMP_OFFSET)) {
        return false;
    }
    if (!writer_append_cstr(
            writer,
            "signature: lifecycle_introspect_sig_t\n"
            "signature: lifecycle_get_provenance_sig_t\n")) {
        return false;
    }

    return true;
}

lifecycle_doc_result_t
lifecycle_generate_documentation(
    lifecycle_documentation_t *doc)
{
    lifecycle_doc_writer_t dry_run;
    lifecycle_doc_writer_t write_run;

    if (doc == NULL)
        return LIFECYCLE_DOC_FAILED;
    if (doc->buffer == NULL) {
        doc->length = 0U;
        return LIFECYCLE_DOC_FAILED;
    }
    if (doc->capacity == 0U) {
        doc->length = 0U;
        return LIFECYCLE_DOC_FAILED;
    }
    if (!lifecycle_documentation_required_inputs_present()) {
        doc->length = 0U;
        return LIFECYCLE_DOC_FAILED;
    }

    dry_run.buffer = NULL;
    dry_run.capacity = (size_t)-1;
    dry_run.length = 0U;
    if (!lifecycle_documentation_render(&dry_run)) {
        doc->length = 0U;
        return LIFECYCLE_DOC_FAILED;
    }

    if (dry_run.length > doc->capacity) {
        doc->length = 0U;
        return LIFECYCLE_DOC_FAILED;
    }

    write_run.buffer = doc->buffer;
    write_run.capacity = doc->capacity;
    write_run.length = 0U;
    if (!lifecycle_documentation_render(&write_run)) {
        doc->length = 0U;
        return LIFECYCLE_DOC_FAILED;
    }

    doc->length = write_run.length;
    return LIFECYCLE_DOC_OK;
}
