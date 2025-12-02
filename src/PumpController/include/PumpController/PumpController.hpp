#pragma once
#include "UI/Motor.hpp"

namespace pump_control {

class PumpController {
public:
    PumpController(ui::Motor& motor);

    void start();
    void stop();
    void up();
    void down();

    void update();  // called each loop to apply motor logic
    int getPower() const;

private:
    ui::Motor& motor;
    int power = 0;
    const int step = 10;

    void applyMotorLogic();
};

} // namespace pump_control
