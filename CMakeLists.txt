cmake_minimum_required(VERSION 3.13)

#add_definitions(-DDEBUG)

if(NOT DEFINED PICO_SDK_PATH)
    message(FATAL_ERROR
            "PICO_SDK_PATH was not specified.
             pico_sdk_init() must first be invoked.")
endif()

project(pico.async-uart)

# Use modern conventions like std::invoke
set(CMAKE_CXX_STANDARD 17)

add_library(soft_uart
    src/soft_uart.cpp
)

add_library(uart_nonblocking
    src/uart_nonblocking.cpp
)

add_library(pio_uart
    src/pio_uart.c
)

pico_generate_pio_header(pio_uart
    ${CMAKE_CURRENT_LIST_DIR}/src/pio_uart.pio
)

# Header file location exposed with target scope for external projects.
target_include_directories(soft_uart PUBLIC inc)
target_include_directories(uart_nonblocking PUBLIC inc)
target_include_directories(pio_uart PUBLIC inc)

target_link_libraries(soft_uart hardware_timer pico_stdlib)
target_link_libraries(uart_nonblocking hardware_dma pico_stdlib)
target_link_libraries(pio_uart hardware_pio hardware_dma pico_stdlib)

if(DEBUG)
    # UART stdio for printf.
    pico_enable_stdio_uart(uart_nonblocking 1)
    pico_enable_stdio_uart(pio_uart 1)
endif()
