/*
 * tests/console/test_lifecycle_governance_provenance_include_ban.c
 *
 * Compile-time include ban verification for core/lifecycle_governance_provenance.c.
 * Pass condition: successful compilation with no forbidden header visibility.
 */

#include <kernul/compiler.h>
#include "../../core/lifecycle_governance_provenance.c"

#ifndef KERNUL_LIFECYCLE_GOVERNANCE_PROVENANCE_H
#error "governance provenance include path must expose lifecycle_governance_provenance.h"
#endif

#ifndef KERNUL_LIFECYCLE_FREEZE_BASELINE_H
#error "governance provenance include path must expose lifecycle_freeze_baseline.h"
#endif

#ifdef KERNUL_LIFECYCLE_GOVERNANCE_INDEX_H
#error "governance provenance include path must not expose governance index headers"
#endif

#ifdef KERNUL_LIFECYCLE_GOVERNANCE_H
#error "governance provenance include path must not expose governance summary headers"
#endif

#ifdef KERNUL_LIFECYCLE_VERIFICATION_H
#error "governance provenance include path must not expose verification headers"
#endif

#ifdef KERNUL_LIFECYCLE_ASSERTION_H
#error "governance provenance include path must not expose assertion headers"
#endif

#ifdef KERNUL_LIFECYCLE_DOCUMENTATION_H
#error "governance provenance include path must not expose documentation headers"
#endif

#ifdef KERNUL_LIFECYCLE_AUDIT_H
#error "governance provenance include path must not expose audit headers"
#endif

#ifdef KERNUL_LIFECYCLE_EXPORT_H
#error "governance provenance include path must not expose export headers"
#endif

#ifdef KERNUL_LIFECYCLE_EXTENSION_H
#error "governance provenance include path must not expose extension headers"
#endif

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)lifecycle_governance_provenance_count;
}
