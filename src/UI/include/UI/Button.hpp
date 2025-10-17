#pragma once

#include "pico/stdlib.h"

#define DEBOUNCE_INTERVAL_MS 50

namespace pump_control::ui {

class Button {
    private:
    int pin;
    volatile bool pressed = false;
    uint64_t lastButtonEvent = 0;
    
    public:
    Button(int pin);
    void init();
    bool hasBeenPressed();
    bool handleInterrupt(uint gpio, uint32_t events);
};

}   //namepsace ui 
