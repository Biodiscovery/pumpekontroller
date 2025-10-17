add_library(ssd1306)
add_library(third_party::ssd1306 ALIAS ssd1306)

target_link_libraries(ssd1306 PRIVATE
    pico_stdlib
    hardware_i2c
)

target_include_directories(ssd1306 PUBLIC include)

target_sources(ssd1306 PRIVATE
    include/ssd1306/font.h
    include/ssd1306/ssd1306.h
    src/ssd1306.c
)
