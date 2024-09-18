3 distinct strategies for a transmitting characters via a nonblocking uart, each with their own use cases.

## Overview

### Soft UART
a software implementation using the Pico's Timer peripheral and a free Alarm.
Under the hood, a finite state machine tracks where we are in the bitstream and when to change the UART TX line.

This strategy is useful for unorthodox slow baud rates where transmission needs to begin as soon as the send command is issued.

### DMA →  Native UART
Leverages DMA to clock out a character array via the native UART peripheral.

This is the recommended strategy for conventional baud rates.

### DMA → PIO UART
Leverages DMA to clock out a character array via a PIO-based UART implementation.

This is the recommended strategy for conventional baud rates where the UART peripheral is not available.
