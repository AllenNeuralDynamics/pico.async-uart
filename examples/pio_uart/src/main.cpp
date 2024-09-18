#include <pico/stdlib.h> // for uart printing
#include <pio_uart.h>
#include <hardware/pio.h>
#include <cstdio> // for printf

// These can be any pins, and they need not be adjacent.
#define UART_TX_PIN (0)
#define BAUD_RATE_BPS (9600)

PIO aux_uart_pio;
uint32_t aux_uart_sm;
uint32_t aux_uart_program_offset;

// Core0 main.
int main()
{
    //stdio_usb_init(); // for printf-style debugging.

    // Setup PIO
    aux_uart_pio = pio0;
    aux_uart_sm = pio_claim_unused_sm(aux_uart_pio, true);
    aux_uart_program_offset = pio_add_program(pio0, &pio_uart_tx_program);
    // SDK 2.0.0 version:
    // pio_claim_free_sm_and_add_program(&pio_uart_tx_program, &aux_uart_pio,
    //                                   &aux_uart_sm, &aux_uart_program_offset);
    pio_uart_tx_program_init(aux_uart_pio, aux_uart_sm, aux_uart_program_offset,
                             UART_TX_PIN, BAUD_RATE_BPS);

    sleep_ms(3000);

    // Cleanup
    pio_sm_unclaim(pio0, aux_uart_sm);
    pio_remove_program(aux_uart_pio, &pio_uart_tx_program,
                       aux_uart_program_offset);
    // SDK 2.0.0 version:
    // pio_remove_program_and_unclaim_sm(&pio_uart_tx_program, aux_uart_pio,
    //                                   aux_uart_sm, aux_uart_program_offset);
}
