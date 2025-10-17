#pragma once

#include <string>

#include "ssd1306/ssd1306.h"

#define I2C_FREQ 400000
#define DISPLAY_SLEEP_INTERVAL 100
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_ADDRESS 0x3C

namespace pump_control::ui {

class Display {
    private:
    int pinSda;
    int pinScl;
    ssd1306_t disp;
    
    public:
    Display(int pinSda, int pinScl);
    void init();
    void showString(const std::string& str);
};

}   //namepsace ui 
