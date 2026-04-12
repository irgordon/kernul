#ifndef KERNUL_LIFECYCLE_EXTENSION_H
#define KERNUL_LIFECYCLE_EXTENSION_H

/*
 * include/kernul/lifecycle_extension.h
 *
 * Kernel-internal inert lifecycle extension declaration surface.
 * Phase 39, Task 1 boundary.
 */

#include <kernul/types.h>

typedef struct lifecycle_extension_decl {
    const char *id;                  /* unique, stable ASCII identifier */
    uint32_t target_freeze_version;  /* version the extension is scoped to */
    const char *description;         /* informational only */
} lifecycle_extension_decl_t;

extern const lifecycle_extension_decl_t lifecycle_extension_declarations[];
extern const size_t lifecycle_extension_declarations_count;

#endif /* KERNUL_LIFECYCLE_EXTENSION_H */
