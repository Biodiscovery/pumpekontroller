#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "hardware/adc.h"

#include "UI/Motor.hpp"
#include "UI/PwmPin.hpp"
#include "UI/RotationSpeedMonitor.hpp"
#include "UI/Motor.hpp"
#include "Utils/Math.hpp"
#include <cmath>

#include "UI/RotationSpeedController.hpp"

namespace pump_control::ui {

    RotationSpeedController::RotationSpeedController(int pinMeasure, int pinMotorA, int pinMotorB) :
        spinMonitor(pinMeasure, 4), motor(pinMotorA, pinMotorB, 20.f, 50.f) {}

    void RotationSpeedController::init() {
        motor.init();
        spinMonitor.init();
    }

    #define ERROR_THRESHOLD 0.05f
    #define POWER_STEP .5f

    void RotationSpeedController::tick() {
        spinMonitor.tick();

        if(fabs(targetRps) <= 0.001f){
            return;
        }

        float measuredRps = spinMonitor.getRps();
        float speedError = measuredRps - fabs(targetRps);
        if(speedError >= ERROR_THRESHOLD){
            motor.setPower(motor.getPower() + POWER_STEP * std::signbit(speedError));
        }

    }

    void RotationSpeedController::setTargetRPS(float target){
        targetRps = target;
        if (targetRps < 0) {
            motor.setDirection(pump_control::ui::Direction::CLOCKWISE);
        }
        else if (targetRps > 0) {
            motor.setDirection(pump_control::ui::Direction::COUNTER_CLOCKWISE);
        }
        else{
            motor.setDirection(pump_control::ui::Direction::OFF);
        }
    }

    float RotationSpeedController::getTargetRPS() const{
        return targetRps;
    }

    float RotationSpeedController::getActualRPS() const{
        return spinMonitor.getRps();
    }

} // namespace pump_control::ui
