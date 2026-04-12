#ifndef KERNUL_LIFECYCLE_EXPORT_H
#define KERNUL_LIFECYCLE_EXPORT_H

/*
 * include/kernul/lifecycle_export.h
 *
 * Kernel-internal deterministic lifecycle audit export serialization.
 * Phase 38, Task 1 boundary.
 */

#include <kernul/types.h>
#include <kernul/lifecycle_audit.h>

typedef enum lifecycle_export_result {
    LIFECYCLE_EXPORT_OK = 0,
    LIFECYCLE_EXPORT_FAILED,
} lifecycle_export_result_t;

typedef struct lifecycle_export_buffer {
    char *buffer;      /* caller-owned storage */
    size_t capacity;   /* total buffer size */
    size_t length;     /* bytes written on success */
} lifecycle_export_buffer_t;

lifecycle_export_result_t
lifecycle_serialize_audit_report(
    const lifecycle_audit_report_t *report,
    lifecycle_export_buffer_t *out_export);

#endif /* KERNUL_LIFECYCLE_EXPORT_H */
