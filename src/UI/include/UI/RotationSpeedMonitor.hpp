#pragma once

#include "pico/stdlib.h"

#define RotationSpeedMonitor_PWM_FREQ 50'000

namespace pump_control::ui {
    
    class RotationSpeedMonitor {

        private:
            int pin;
            int divider;

            uint64_t lastPass = 0;
            float rps = 0.f;

            bool armed = false;
            float emaFast = 0.0f;
            float emaSlow = 0.0f;

            void passedMagnet();


        public:
            RotationSpeedMonitor(int pin, int divider);
            void init();
            void tick();
            float getRpm();
};

} // namespace pump_control:ui
