#pragma once
#include "PumpController/PumpController.hpp"

class WebServer {
public:
    static void init(pump_control::PumpController& controller);
    static void loop();
    static pump_control::PumpController* ctrl;
};