#include <string>

#include "hardware/gpio.h"
#include "hardware/i2c.h"

// #include "ssd1306/ssd1306.h"

#include "UI/Display.hpp"

namespace pump_control::ui {

Display::Display(int pinSda, int pinScl) : pinSda(pinSda), pinScl(pinScl) {} 

void Display::init(){
    i2c_init(i2c0, I2C_FREQ);
    disp.external_vcc=false;
    // ssd1306_init(&disp, DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_ADDRESS, i2c0);
    // ssd1306_clear(&disp);
}

void Display::showString(const std::string& stringToShow){
    sleep_ms(DISPLAY_SLEEP_INTERVAL);
    // ssd1306_clear(&disp);
    // ssd1306_draw_string(&disp, 8, 24, 2, stringToShow.c_str());
    // ssd1306_show(&disp);
}

} // namespace pump_control::ui
