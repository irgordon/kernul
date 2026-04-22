/*
 * tests/console/test_lifecycle_governance_compatibility_include_ban.c
 *
 * Compile-time include ban verification for
 * core/lifecycle_governance_compatibility.c.
 * Pass condition: successful compilation with no forbidden header visibility.
 */

#include <kernul/compiler.h>
#include "../../core/lifecycle_governance_compatibility.c"

#ifndef KERNUL_LIFECYCLE_GOVERNANCE_COMPATIBILITY_H
#error "governance compatibility include path must expose lifecycle_governance_compatibility.h"
#endif

#ifndef KERNUL_LIFECYCLE_FREEZE_BASELINE_H
#error "governance compatibility include path must expose lifecycle_freeze_baseline.h"
#endif

#ifdef KERNUL_LIFECYCLE_GOVERNANCE_INDEX_H
#error "governance compatibility include path must not expose governance index headers"
#endif

#ifdef KERNUL_LIFECYCLE_GOVERNANCE_PROVENANCE_H
#error "governance compatibility include path must not expose governance provenance headers"
#endif

#ifdef KERNUL_LIFECYCLE_GOVERNANCE_H
#error "governance compatibility include path must not expose governance summary headers"
#endif

#ifdef KERNUL_LIFECYCLE_VERIFICATION_H
#error "governance compatibility include path must not expose lifecycle verification headers"
#endif

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)lifecycle_governance_compatibility_count;
}
