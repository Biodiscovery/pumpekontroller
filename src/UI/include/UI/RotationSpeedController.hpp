#pragma once

#include "UI/RotationSpeedMonitor.hpp"
#include "UI/Motor.hpp"
#include "pico/stdlib.h"

#define RotationSpeedController_PWM_FREQ 50'000

namespace pump_control::ui {
    
    class RotationSpeedController {

        private:
            Motor motor;
            RotationSpeedMonitor spinMonitor;

            float targetRps = 0.f;

        public:
            RotationSpeedController(int pinMeasure, int pinMotorA, int pinMotorB);
            void init();
            void tick();

            float getTargetRPS() const;
            float getActualRPS() const;

            void setTargetRPS(float target);
};

} // namespace pump_control:ui
