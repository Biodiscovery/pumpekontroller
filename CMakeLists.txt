cmake_minimum_required(VERSION 3.13...3.27)

include(${CMAKE_SOURCE_DIR}/cmake/pico_sdk_import.cmake)

project(pump_control CXX C ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

pico_sdk_init()

set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

add_executable(pump_control)

add_subdirectory(src)

target_sources(pump_control PRIVATE
    src/main.cpp
    src/board.hpp
    src/PumpController/include/PumpController/PumpController.hpp
    src/PumpController/src/PumpController.cpp
)

target_include_directories(pump_control PUBLIC src/PumpController/include)

target_link_libraries(pump_control PRIVATE
    pico_stdlib
    hardware_i2c
    hardware_pwm
    # hardware_flash
)

target_link_libraries(pump_control PRIVATE
    third_party::ssd1306
)
    
target_link_libraries(pump_control PRIVATE
    pump_control::ui
)


if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to DEBUG")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

pico_enable_stdio_usb(pump_control 1)
pico_enable_stdio_uart(pump_control 0)

pico_add_extra_outputs(pump_control)

target_link_options(pump_control PRIVATE -u _printf_float)

# Reserve 4KB at the end of flash for config storage. Required for using NVS/hardware_flash.
# add_link_options(-Wl,--defsym=__flash_config_start=0x001f0000)

# Symlink compile_commands.json to build folder for convenience.
add_custom_command(
    TARGET pump_control POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${PROJECT_SOURCE_DIR}/compile_commands.json"
        "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json"
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${PROJECT_SOURCE_DIR}/upload.sh"
        "${CMAKE_CURRENT_BINARY_DIR}/upload.sh"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
