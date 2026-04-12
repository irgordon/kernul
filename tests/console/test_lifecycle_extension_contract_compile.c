/*
 * tests/console/test_lifecycle_extension_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/lifecycle_extension.h.
 * Pass condition: successful compilation.
 */

#include <kernul/lifecycle_extension.h>
#include <kernul/compiler.h>

#ifdef KERNUL_SESSION_H
#error "lifecycle_extension.h must not include session internals"
#endif

#ifdef KERNUL_SCHED_H
#error "lifecycle_extension.h must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "lifecycle_extension.h must not include scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "lifecycle_extension.h must not include scheduler container headers"
#endif

#ifdef KERNUL_LIFECYCLE_FREEZE_BASELINE_H
#error "lifecycle_extension.h must not include freeze baseline headers"
#endif

KERN_STATIC_ASSERT(sizeof(((lifecycle_extension_decl_t *)0)->target_freeze_version) == sizeof(uint32_t),
                   "lifecycle extension target_freeze_version type drift");

static const lifecycle_extension_decl_t *const check_lifecycle_extension_declarations =
    lifecycle_extension_declarations;
static const size_t *const check_lifecycle_extension_declarations_count =
    &lifecycle_extension_declarations_count;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    lifecycle_extension_decl_t decl = {0};

    (void)check_lifecycle_extension_declarations;
    (void)check_lifecycle_extension_declarations_count;
    (void)decl.id;
    (void)decl.target_freeze_version;
    (void)decl.description;
}
