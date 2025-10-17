#include "hardware/gpio.h"

#include "UI/RgbLed.hpp"

namespace pump_control::ui {

RgbLed::RgbLed(int pinR, int pinG, int pinB) : pinR(pinR), pinG(pinG), pinB(pinB) {} 

void RgbLed::init(){
    gpio_init(pinR);
    gpio_init(pinG);
    gpio_init(pinB);
    gpio_set_dir(pinR, GPIO_OUT);
    gpio_set_dir(pinG, GPIO_OUT);
    gpio_set_dir(pinB, GPIO_OUT);
}

void RgbLed::setColor(const RgbColor& color){
    gpio_put(pinR, color.r);
    gpio_put(pinG, color.g);
    gpio_put(pinB, color.b);
}

} // namespace pump_control::ui
