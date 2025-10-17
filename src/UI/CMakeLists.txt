
add_library(ui)
add_library(pump_control::ui ALIAS ui)

target_link_libraries(ui PRIVATE
    hardware_adc
    hardware_clocks
    hardware_gpio
    hardware_i2c
    hardware_pwm
    pico_stdlib
    # third_party::ssd1306
)

target_link_libraries(ui PUBLIC
    third_party::ssd1306
)

target_include_directories(ui PUBLIC include)

target_sources(ui PRIVATE
    include/UI/Button.hpp
    include/UI/Display.hpp
    include/UI/Joystick.hpp
    include/UI/PwmPin.hpp
    include/UI/RgbLed.hpp
    include/UI/Switch.hpp
    src/Button.cpp
    src/Display.cpp
    src/Joystick.cpp
    src/PwmPin.cpp
    src/RgbLed.cpp
    src/Switch.cpp
)
