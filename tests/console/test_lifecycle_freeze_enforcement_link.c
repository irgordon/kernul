/*
 * tests/console/test_lifecycle_freeze_enforcement_link.c
 *
 * Link-check for lifecycle freeze enforcement dependency boundary symbols.
 * Pass condition: successful link against freeze enforcement implementation.
 */

#include <kernul/lifecycle_freeze_enforcement.h>
#include <kernul/lifecycle_freeze_baseline.h>

int main(void)
{
    if (lifecycle_freeze_enforcement_link_anchor != LIFECYCLE_FREEZE_VERSION)
        return 1;

    return 0;
}
