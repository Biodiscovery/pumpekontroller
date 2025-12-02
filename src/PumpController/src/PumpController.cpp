#include "PumpController/PumpController.hpp"
#include <cstdio>
#include <cmath>

namespace pump_control {

PumpController::PumpController(ui::Motor& motor) 
    : motor(motor) {}

void PumpController::start() {
    if (power == 0) power = 20;  // default start
}

void PumpController::stop() {
    power = 0;
}

void PumpController::up() {
    power += step;
    if (power > 100) power = 100;
}

void PumpController::down() {
    power -= step;
    if (power < -100) power = -100;
}

int PumpController::getPower() const {
    return power;
}

void PumpController::update() {
    applyMotorLogic();
}

void PumpController::applyMotorLogic() {
    if (power < 0) {
        motor.setDirection(ui::Direction::CLOCKWISE);
    } else if (power > 0) {
        motor.setDirection(ui::Direction::COUNTER_CLOCKWISE);
    } else {
        motor.setDirection(ui::Direction::OFF);
    }

    motor.setPower(std::abs(power));
}

} // namespace pump_control
