
add_library(drivers)
add_library(pump_control::drivers ALIAS drivers)

target_link_libraries(drivers PRIVATE
    hardware_adc
    hardware_clocks
    hardware_gpio
    hardware_i2c
    hardware_pwm
    pico_stdlib
    # third_party::ssd1306
)

target_link_libraries(drivers PUBLIC
    third_party::ssd1306
)

target_include_directories(drivers PUBLIC include)

target_sources(drivers PRIVATE
    include/drivers/Button.hpp
    include/drivers/Display.hpp
    include/drivers/Joystick.hpp
    include/drivers/PwmPin.hpp
    include/drivers/RgbLed.hpp
    include/drivers/Switch.hpp
    src/Button.cpp
    src/Display.cpp
    src/Joystick.cpp
    src/PwmPin.cpp
    src/RgbLed.cpp
    src/Switch.cpp
)
