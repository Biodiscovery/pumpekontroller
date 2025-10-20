#include <cstdio>
// #include <cstring>
#include <cmath>
#include "UI/include/UI/Motor.hpp"
#include "hardware/gpio.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"

#include "board.hpp"
#include "UI/Button.hpp"
// #include "PumpController/PumpController.hpp"
#include "UI/PwmPin.hpp"
#include "UI/Motor.hpp"

// #define RX_BUF_SIZE 4
// #define MSG_BUF_SIZE 64

using namespace pump_control;

// PumpController PumpController;

ui::Button btn1(PIN_BTN_1);
ui::Button btn2(PIN_BTN_2);
ui::Motor motor(PIN_MOTOR_A, PIN_MOTOR_B, 20.f, 50.f);

void start() {
  printf("----- Akselpumpe -----\n");
  // PumpController.init();
  btn1.init();
  btn2.init();
  motor.init();
  sleep_ms(1000);
  printf("READY.\n\n");
}

void tick() {
  //
}

int main() {
  stdio_init_all();
  sleep_ms(1000);
  
  start();

  motor.setPower(80);
  ui::Direction dir; 
  
  while (true) {

    static int power = 0;
    constexpr int step = 10;

    if (btn1.hasBeenPressed()) {
      power = power - step;
    }

    if (btn2.hasBeenPressed()) {
      power = power + step;
    }

    dir = motor.getDirection();

    if (power < 0 && dir == pump_control::ui::Direction::COUNTER_CLOCKWISE) {
      motor.setDirection(pump_control::ui::Direction::CLOCKWISE);
    }
    if (power > 0 && dir == pump_control::ui::Direction::CLOCKWISE) {
      motor.setDirection(pump_control::ui::Direction::COUNTER_CLOCKWISE);
    }

    motor.setPower(static_cast<float>(abs(power)));

  }
}
