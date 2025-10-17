#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "pico/time.h"

#include "UI/Joystick.hpp"
#include "UI/Button.hpp"

float adcRawToVoltage(uint16_t raw){
    return raw * (3.3f / 4095.f);
}

float voltageToAngle(float voltage){
    return (voltage / 3.3f) * 100.f - 50.f;
}

namespace pump_control::ui {

Joystick::Joystick(int pinX, int pinY, int pinBtn) : pinX(pinX), pinY(pinY), btn(pinBtn) {} 

void Joystick::init(){
    btn.init();
    adc_init();
    adc_gpio_init(pinX);
    adc_gpio_init(pinY);
}

bool Joystick::buttonPressed(){
    return btn.hasBeenPressed();
}


float Joystick::getX(){
    adc_select_input(0);
    return voltageToAngle(adcRawToVoltage(adc_read()));
}

float Joystick::getY(){
    adc_select_input(1);
    return voltageToAngle(adcRawToVoltage(adc_read()));
}


} // namespace pump_control::ui
