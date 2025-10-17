#include <cstdio>
#include <vector>

#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

#include "UI/PwmPin.hpp"

// static std::vector<pump_control::ui::PwmPin*> pwmPins;

static int map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// void on_pwm_wrap() {
//     for(pump_control::ui::PwmPin *pin : pwmPins){
//         pin->pinOnWrapCb();
//     }
// }

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
        // pwm_clear_irq(slice);
        // pwm_set_irq_enabled(slice, true);
        // irq_set_exclusive_handler(PWM_DEFAULT_IRQ_NUM(), on_pwm_wrap);
        // irq_set_enabled(PWM_DEFAULT_IRQ_NUM(), true);

        config = pwm_get_default_config();
        pwm_config_set_clkdiv(&config, clockdiv);
        pwm_init(slice, &config, true);

        setFrequency(freq);
        setDutyCycle(0);

        // pwmPins.emplace_back(this);
    }

    // void PwmPin:: pinOnWrapCb(){
    //     //
    // }

    // void PwmPin::setNotMillis(float notMillis){
    //     pwm_set_gpio_level(pin, (notMillis / 20'000.F) * wrap);
    // }

} // namespace pump_control::ui
