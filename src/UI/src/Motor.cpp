#include "pico/stdlib.h"

#include "UI/Motor.hpp"
#include "UI/PwmPin.hpp"
#include "Utils/Math.hpp"

namespace pump_control::ui {

    Motor::Motor(int pinA, int pinB, float minDuty, float maxDuty, Direction initialDirection)
            : pwmA(pinA, 50'000), pwmB(pinB, 50'000), dir(initialDirection), minDuty(minDuty), maxDuty(maxDuty){}

    void Motor::setPower(float percentage) {
        if(percentage < 0.f){
            percentage = 0;
        }
        else if(percentage > 100.f){
            percentage = 100.f;
        }

        float adjustedPercentage = utils::mapf(percentage, 0, 100, minDuty, maxDuty);

        if(dir == Direction::CLOCKWISE){
            pwmB.setDutyCycle(0);
            pwmA.setDutyCycle(adjustedPercentage);
        }
        else if(dir == Direction::COUNTER_CLOCKWISE){
            pwmA.setDutyCycle(0);
            pwmB.setDutyCycle(adjustedPercentage);
        }
        else{
            pwmA.setDutyCycle(0);
            pwmB.setDutyCycle(0);
        }
    };

    void Motor::setDirection(const Direction& dir){
        this->dir = dir;
    }

    void Motor::init() {
        pwmA.init();
        pwmB.init();
    }

} // namespace pump_control::ui
