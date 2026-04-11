#ifndef KERNUL_LIFECYCLE_DOCUMENTATION_H
#define KERNUL_LIFECYCLE_DOCUMENTATION_H

/*
 * include/kernul/lifecycle_documentation.h
 *
 * Kernel-internal deterministic lifecycle documentation synthesis.
 * Phase 36, Task 1 boundary.
 */

#include <kernul/types.h>

typedef enum lifecycle_doc_result {
    LIFECYCLE_DOC_OK = 0,
    LIFECYCLE_DOC_FAILED,
} lifecycle_doc_result_t;

typedef struct lifecycle_documentation {
    char *buffer;      /* caller-owned storage */
    size_t capacity;   /* total buffer size */
    size_t length;     /* bytes written on success */
} lifecycle_documentation_t;

lifecycle_doc_result_t
lifecycle_generate_documentation(
    lifecycle_documentation_t *doc);

#endif /* KERNUL_LIFECYCLE_DOCUMENTATION_H */
