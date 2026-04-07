/*
 * tests/console/test_lifecycle_assertion_link.c
 *
 * Link-check for lifecycle assertion dependency boundary symbols.
 * Pass condition: successful link against assertion implementation.
 */

#include <kernul/lifecycle_assertion.h>

int main(void)
{
    lifecycle_snapshot_t snapshot = {0};
    lifecycle_provenance_t provenance = {0};
    lifecycle_boundary_t boundary = {0};

    boundary.require_verified = true;

    (void)&lifecycle_assert;
    (void)lifecycle_assert(&snapshot,
                           LIFECYCLE_VERIFY_VERIFIED,
                           &provenance,
                           &boundary);
    return 0;
}
