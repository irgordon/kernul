/*
 * Fixed q35 COM1 polling backend for bootstrap diagnostics only.
 * The single boot CPU owns UART configuration and transmission. No shared
 * software state, IRQ handler, allocation, input, or runtime console policy.
 */

#include "boot_console.h"
#include <kernul/errors.h>

#define UART_COM1_BASE 0x3f8U
#define UART_DATA 0U
#define UART_INTERRUPT_ENABLE 1U
#define UART_FIFO_CONTROL 2U
#define UART_LINE_CONTROL 3U
#define UART_MODEM_CONTROL 4U
#define UART_LINE_STATUS 5U
#define UART_DIVISOR_LOW 0U
#define UART_DIVISOR_HIGH 1U
#define UART_INTERRUPTS_DISABLED 0U
#define UART_LINE_8N1 0x03U
#define UART_DIVISOR_ACCESS 0x80U
#define UART_DIVISOR_115200 1U
#define UART_DIVISOR_HIGH_ZERO 0U
#define UART_FIFO_ENABLE_CLEAR 0x07U
#define UART_DTR_RTS 0x03U
#define UART_HOLDING_EMPTY 0x20U
#define UART_TRANSMITTER_EMPTY 0x40U
#define UART_UNMAPPED_STATUS 0xffU
#define UART_POLL_LIMIT 100000U

static kern_err_t transmit_byte(u8 byte);
static kern_err_t drain_transmitter(kern_err_t status);
static kern_err_t wait_for_status(u8 required);
static bool status_is_pending(u8 required, u32 remaining);
static bool transmitter_is_ready(u8 required);
static kern_err_t poll_result(u32 remaining);
static bool message_is_pending(const char *message, kern_err_t status);
static u8 read_register(u16 offset);
static void write_register(u16 offset, u8 value);
static u8 read_port8(u16 port);
static void write_port8(u16 port, u8 value);

void arch_boot_console_init(void)
{
    /* Clear inherited DLAB before masking interrupts through the shared port. */
    write_register(UART_LINE_CONTROL, UART_LINE_8N1);
    write_register(UART_INTERRUPT_ENABLE, UART_INTERRUPTS_DISABLED);
    write_register(UART_LINE_CONTROL, UART_LINE_8N1 | UART_DIVISOR_ACCESS);
    write_register(UART_DIVISOR_LOW, UART_DIVISOR_115200);
    write_register(UART_DIVISOR_HIGH, UART_DIVISOR_HIGH_ZERO);
    write_register(UART_LINE_CONTROL, UART_LINE_8N1);
    write_register(UART_FIFO_CONTROL, UART_FIFO_ENABLE_CLEAR);
    write_register(UART_MODEM_CONTROL, UART_DTR_RTS);
}

kern_err_t arch_boot_console_write(const char *message)
{
    kern_err_t status = KERN_OK;

    while (message_is_pending(message, status)) {
        status = transmit_byte((u8)*message);
        message++;
    }
    return drain_transmitter(status);
}

static kern_err_t transmit_byte(u8 byte)
{
    kern_err_t status = wait_for_status(UART_HOLDING_EMPTY);

    if (status != KERN_OK)
        return status;
    write_register(UART_DATA, byte);
    return KERN_OK;
}

static kern_err_t drain_transmitter(kern_err_t status)
{
    if (status != KERN_OK)
        return status;
    /* THRE alone does not prove that the final byte left the shift register. */
    return wait_for_status(UART_TRANSMITTER_EMPTY);
}

static kern_err_t wait_for_status(u8 required)
{
    u32 remaining = UART_POLL_LIMIT;

    while (status_is_pending(required, remaining))
        remaining--;
    return poll_result(remaining);
}

static bool status_is_pending(u8 required, u32 remaining)
{
    return remaining != 0U && !transmitter_is_ready(required);
}

static bool transmitter_is_ready(u8 required)
{
    u8 status = read_register(UART_LINE_STATUS);

    if (status == UART_UNMAPPED_STATUS)
        return false;
    return (status & required) == required;
}

static kern_err_t poll_result(u32 remaining)
{
    if (remaining == 0U)
        return KERN_EIO;
    return KERN_OK;
}

static bool message_is_pending(const char *message, kern_err_t status)
{
    return status == KERN_OK && *message != '\0';
}

static u8 read_register(u16 offset)
{
    return read_port8(UART_COM1_BASE + offset);
}

static void write_register(u16 offset, u8 value)
{
    write_port8(UART_COM1_BASE + offset, value);
}

static u8 read_port8(u16 port)
{
    u8 value;

    __asm__ volatile("inb %1, %0" : "=a"(value) : "Nd"(port) : "memory");
    return value;
}

static void write_port8(u16 port, u8 value)
{
    __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port) : "memory");
}
