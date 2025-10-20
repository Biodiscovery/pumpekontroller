#pragma once

#include <cstdio>

#include "hardware/pwm.h"
#include "pico/stdlib.h"

#include "UI/PwmPin.hpp"

#define MOTOR_PWM_FREQ 50'000

namespace pump_control::ui {
    
    enum class Direction {
        CLOCKWISE,
        COUNTER_CLOCKWISE,
        OFF,
    };
    
    class Motor {
        private:
        PwmPin pwmA;
        PwmPin pwmB;
        
        float minDuty;
        float maxDuty;
        float power;
        
        Direction dir;
        bool enabled;

        public:
            Motor(int pinA, int pinB,
                float minDuty = 0, float maxDuty = 100,
                Direction initialDirection = Direction::OFF);
            void init();
            void setPower(float);
            void setDirection(const Direction& dir);
            const Direction& getDirection() const;
            float getPower() const;

};

} // namespace pump_control:ui
