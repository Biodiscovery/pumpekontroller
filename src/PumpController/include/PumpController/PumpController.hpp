#pragma once

namespace pump_control {

class PumpController {
private:

public:
  PumpController();
  void init();
  void tick();
};

} // namespace pump_control
