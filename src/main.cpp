#include <cstdio>
// #include <cstring>
#include <cmath>
#include "UI/include/UI/Motor.hpp"
#include "UI/include/UI/RotationSpeedMonitor.hpp"
#include "hardware/gpio.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"

#include "board.hpp"
#include "UI/Button.hpp"
// #include "PumpController/PumpController.hpp"
#include "UI/PwmPin.hpp"
#include "UI/Motor.hpp"
#include "UI/RotationSpeedMonitor.hpp"

// #define RX_BUF_SIZE 4
// #define MSG_BUF_SIZE 64

using namespace pump_control;

// PumpController PumpController;

ui::Button btn1(PIN_BTN_1);
ui::Button btn2(PIN_BTN_2);
ui::RotationSpeedMonitor spinMonitor(PIN_HALL_EFFECT, 4);
ui::Motor motor(PIN_MOTOR_A, PIN_MOTOR_B, 20.f, 50.f);

void start() {
  printf("----- Akselpumpe -----\n");
  // PumpController.init();
  btn1.init();
  btn2.init();
  motor.init();
  spinMonitor.init();
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
  ui::Direction dir = pump_control::ui::Direction::COUNTER_CLOCKWISE; 
  
  while (true) {

    spinMonitor.tick();

    static int power = 50;
    constexpr int step = 10;

    if (btn1.hasBeenPressed()) {
      power = power - step;
    }

    if (btn2.hasBeenPressed()) {
      power = power + step;
    }

    dir = motor.getDirection();

    if (power < 0) {
      motor.setDirection(pump_control::ui::Direction::CLOCKWISE);
    }
    else if (power > 0) {
      motor.setDirection(pump_control::ui::Direction::COUNTER_CLOCKWISE);
    }
    else{
      motor.setDirection(pump_control::ui::Direction::OFF);
    }

    // printf("Power: %d\n", power);
    static uint64_t printCounter = 0;
    static uint64_t lastPrint = 0;
    #define PRINT_INTERVAL_MS 100

    printCounter = time_us_64();
    bool shouldPrint = printCounter - lastPrint > PRINT_INTERVAL_MS * 1000;
    // shouldPrint = true;
    if(shouldPrint){
      lastPrint = printCounter;
      printf("RPS: %.1f\n", spinMonitor.getRpm());
    }

    motor.setPower(abs(power));

  }
}
