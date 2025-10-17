#include <vector>

#include "hardware/gpio.h"
#include "pico/time.h"

#include "UI/Button.hpp"

static std::vector<pump_control::ui::Button*> buttons;

static void gpio_callback(uint gpio, uint32_t events){
    for(pump_control::ui::Button* btn : buttons){
        if(btn->handleInterrupt(gpio, events)){
            return;
        }
    }
}

namespace pump_control::ui {

Button::Button(int pin) : pin(pin) {} 

bool Button::handleInterrupt(uint gpio, uint32_t events){
    if(gpio != pin){
        return false;
    }
    if(events == GPIO_IRQ_EDGE_FALL && !pressed){
        lastButtonEvent = time_us_64();
        pressed = true;
    }
    else if(events == GPIO_IRQ_EDGE_RISE){
        lastButtonEvent = 0;
        pressed = false;
    }
    return true;
}

void Button::init(){
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
    buttons.emplace_back(this);
    gpio_set_irq_enabled_with_callback(pin, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
}

bool Button::hasBeenPressed(){
    if(pressed && lastButtonEvent + DEBOUNCE_INTERVAL_MS <= time_us_64() && !gpio_get(pin)){
        pressed = false;
        lastButtonEvent = 0;
        return true;
    }
    return false;
}

} // namespace pump_control::ui
