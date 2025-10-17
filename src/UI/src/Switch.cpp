#include "hardware/gpio.h"

#include "UI/Switch.hpp"

namespace pump_control::ui {

Switch::Switch(int pin) : pin(pin) {} 

void Switch::init(){
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
}

bool Switch::read(){
    return gpio_get(pin);
}

} // namespace pump_control::ui
