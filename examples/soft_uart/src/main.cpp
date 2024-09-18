#include <pico/stdlib.h> // for uart printing
#include <soft_uart.h>
#include <cstdio> // for printf

// These can be any pins, and they need not be adjacent.
#define UART_TX_PIN (0)
#define BAUD_RATE_BPS (9600)

// Core0 main.
int main()
{
    //stdio_usb_init(); // for printf-style debugging.

    // Setup soft uart.
    SoftUART uart = SoftUART(UART_TX_PIN, BAUD_RATE_BPS);
    char my_str[] = "Hello, world!";

    while (true)
    {
        uart.send((uint8_t*)my_str, sizeof(my_str));

        while (uart.is_busy())
        {
            if (uart.requires_update())
                uart.update();
        }
        sleep_ms(500);
    }
}
