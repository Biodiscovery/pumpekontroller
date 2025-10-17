#include "UI/PwmPin.hpp"

#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"


namespace pump_control::ui {

    PwmPin::PwmPin(int pin, float freq) : pin(pin), freq(freq), slice(pwm_gpio_to_slice_num(pin)) {}

    void PwmPin::setDutyCycle(float dutyPercent) {
        float newDuty = dutyPercent / 100.f;
        if(duty == newDuty){
            return;
        }
        this->duty = newDuty;
        float level = wrap * duty;
        pwm_set_enabled(slice, false);
        pwm_set_gpio_level(pin, level);
        pwm_set_enabled(slice, true);
        printf("Duty: %f\tLevel: %f\n", duty, level);
    };
    
    void PwmPin::setFrequency(float freq) {
        this->freq = freq;
        this->wrap = fc / freq - 1;
        pwm_set_enabled(slice, false);
        pwm_set_wrap(slice, wrap);
        pwm_set_enabled(slice, true);
        printf("Frequency: %f\tWrap: %f\n", freq, wrap);
    };

    void PwmPin::init() {
        gpio_set_function(pin, GPIO_FUNC_PWM);

        config = pwm_get_default_config();
        pwm_config_set_clkdiv(&config, clockdiv);
        pwm_init(slice, &config, true);

        setFrequency(freq);
        setDutyCycle(0);
    }

} // namespace pump_control::ui
