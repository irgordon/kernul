/*
 * tests/console/test_lifecycle_verification_link.c
 *
 * Link-check for lifecycle verification dependency boundary symbols.
 * Pass condition: successful link against verification implementation.
 */

#include <kernul/lifecycle_verification.h>

int main(void)
{
    lifecycle_snapshot_t snapshot = {0};

    (void)&lifecycle_verify;
    (void)lifecycle_verify(&snapshot);
    return 0;
}
