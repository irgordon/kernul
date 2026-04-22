/*
 * tests/console/test_lifecycle_governance_schema_include_ban.c
 *
 * Compile-time include ban verification for
 * core/lifecycle_governance_schema.c.
 * Pass condition: successful compilation with no forbidden header visibility.
 */

#include <kernul/compiler.h>
#include "../../core/lifecycle_governance_schema.c"

#ifndef KERNUL_LIFECYCLE_GOVERNANCE_SCHEMA_H
#error "governance schema include path must expose lifecycle_governance_schema.h"
#endif

#ifdef KERNUL_LIFECYCLE_FREEZE_BASELINE_H
#error "governance schema include path must not expose lifecycle_freeze_baseline.h"
#endif

#ifdef KERNUL_LIFECYCLE_GOVERNANCE_INDEX_H
#error "governance schema include path must not expose governance index headers"
#endif

#ifdef KERNUL_LIFECYCLE_GOVERNANCE_PROVENANCE_H
#error "governance schema include path must not expose governance provenance headers"
#endif

#ifdef KERNUL_LIFECYCLE_GOVERNANCE_COMPATIBILITY_H
#error "governance schema include path must not expose governance compatibility headers"
#endif

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)lifecycle_governance_schema_count;
}
