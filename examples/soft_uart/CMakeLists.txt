cmake_minimum_required(VERSION 3.13)

# PICO_SDK_PATH must be defined.
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

set(CMAKE_CXX_STANDARD 17)

project(soft_uart_example)

pico_sdk_init()
add_subdirectory(../../ build) # Path to the library's CMakeLists.txt

add_executable(${PROJECT_NAME}
    src/main.cpp
)

include_directories(inc)

target_link_libraries(${PROJECT_NAME} soft_uart pico_stdlib)

pico_add_extra_outputs(${PROJECT_NAME})

pico_enable_stdio_usb(${PROJECT_NAME} 1) # UART stdio for printf.
