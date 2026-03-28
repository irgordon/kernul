/*
 * tests/console/test_interactive_admission_link.c
 *
 * Link-check for interactive scheduler admission contract symbols.
 * Pass condition: successful link.
 */

#include <kernul/interactive_admission.h>

int main(void)
{
    (void)&interactive_admission_admit;
    (void)&interactive_admission_state;
    return 0;
}
