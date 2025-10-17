#include <cstdio>

#include "hardware/pwm.h"
#include "pico/stdlib.h"

namespace pump_control::ui {


class PwmPin {

    static constexpr float f0 = 150'000'000.f;
    static constexpr float clockdiv = 4.f;
    static constexpr float fc = f0 / clockdiv;

    private:
        int pin;
        int slice;

        float duty;
        float freq;
        float wrap;

        pwm_config config;
        
        public:
        PwmPin(int pin, float freq);
        void init();
        // void setNotMillis(float notMillis);
        // void pinOnWrapCb();
        void setDutyCycle(float dutyPercent);
        void setFrequency(float freq);
};

} // namespace pump_control:ui
