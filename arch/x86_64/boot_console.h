/* Architecture-private output for the q35 COM1 bootstrap console. */
#ifndef X86_64_BOOT_CONSOLE_H
#define X86_64_BOOT_CONSOLE_H

#include <kernul/types.h>

/*
 * Configure the fixed 16550-compatible COM1 at 0x3f8 for 115200 baud, 8N1.
 * The boot CPU owns the UART registers exclusively, with interrupts disabled;
 * terminal exception entry may reinitialize it. UART interrupts stay disabled.
 * No locks, allocation, sleep, firmware calls, or hardware discovery.
 */
void arch_boot_console_init(void);

/*
 * Synchronously transmit a borrowed, readable NUL-terminated message after
 * initialization. No formatting or ownership transfer. Same caller context
 * as init; no concurrent writers or input support.
 * Busy-polls at most 100000 status reads per byte and final transmitter drain.
 * This is a CPU-work bound, not a timer-based deadline. Returns KERN_OK when
 * fully transmitted, or KERN_EIO on timeout; a prefix may already be sent.
 */
kern_err_t arch_boot_console_write(const char *message);

#endif /* X86_64_BOOT_CONSOLE_H */
