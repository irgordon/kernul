/*
 * tests/console/test_lifecycle_extension_include_ban.c
 *
 * Compile-time include ban verification for core/lifecycle_extension.c.
 * Pass condition: successful compilation with no forbidden header visibility.
 */

#include <kernul/compiler.h>
#include "../../core/lifecycle_extension.c"

#ifndef KERNUL_LIFECYCLE_EXTENSION_H
#error "lifecycle extension include path must expose lifecycle_extension.h"
#endif

#ifndef KERNUL_LIFECYCLE_FREEZE_BASELINE_H
#error "lifecycle extension include path must expose lifecycle_freeze_baseline.h"
#endif

#ifdef KERNUL_LIFECYCLE_VERIFICATION_H
#error "lifecycle extension include path must not expose verification headers"
#endif

#ifdef KERNUL_LIFECYCLE_ASSERTION_H
#error "lifecycle extension include path must not expose assertion headers"
#endif

#ifdef KERNUL_LIFECYCLE_FREEZE_ENFORCEMENT_H
#error "lifecycle extension include path must not expose freeze enforcement headers"
#endif

#ifdef KERNUL_LIFECYCLE_DOCUMENTATION_H
#error "lifecycle extension include path must not expose documentation headers"
#endif

#ifdef KERNUL_LIFECYCLE_AUDIT_H
#error "lifecycle extension include path must not expose audit headers"
#endif

#ifdef KERNUL_LIFECYCLE_EXPORT_H
#error "lifecycle extension include path must not expose export headers"
#endif

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)lifecycle_extension_declarations_count;
}
