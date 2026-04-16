/*
 * tests/console/test_lifecycle_governance_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/lifecycle_governance.h.
 * Pass condition: successful compilation.
 */

#include <kernul/lifecycle_governance.h>
#include <kernul/compiler.h>

#ifdef KERNUL_LIFECYCLE_FREEZE_BASELINE_H
#error "lifecycle_governance.h must not include freeze baseline headers"
#endif

#ifdef KERNUL_SESSION_H
#error "lifecycle_governance.h must not include session internals"
#endif

#ifdef KERNUL_SCHED_H
#error "lifecycle_governance.h must not include scheduler headers"
#endif

KERN_STATIC_ASSERT(sizeof(((lifecycle_governance_summary_t *)0)->freeze_version) == sizeof(uint32_t),
                   "lifecycle governance freeze_version type drift");

static const lifecycle_governance_summary_t *const check_lifecycle_governance_summary =
    &lifecycle_governance_summary;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    lifecycle_governance_summary_t summary = {0};

    (void)check_lifecycle_governance_summary;

    (void)summary.freeze_version;
    (void)summary.introspection_surface;
    (void)summary.verification_surface;
    (void)summary.provenance_surface;
    (void)summary.assertion_surface;
    (void)summary.documentation_surface;
    (void)summary.audit_surface;
    (void)summary.export_surface;
    (void)summary.extension_surface;
}
