#include <cstdio>

#include "hardware/gpio.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"

#include "board.hpp"
#include "UI/Button.hpp"
// #include "PumpController/PumpController.hpp"
#include "UI/PwmPin.hpp"
#include "UI/Motor.hpp"
#include "UI/RotationSpeedController.hpp"

// #define RX_BUF_SIZE 4
// #define MSG_BUF_SIZE 64

using namespace pump_control;

// PumpController PumpController;

ui::Button btn1(PIN_BTN_1);
ui::Button btn2(PIN_BTN_2);
ui::RotationSpeedController speedController(PIN_HALL_EFFECT, PIN_MOTOR_A, PIN_MOTOR_B);

void start() {
  printf("----- Akselpumpe -----\n");
  // PumpController.init();
  btn1.init();
  btn2.init();
  speedController.init();
  sleep_ms(1000);
  printf("-- READY. --\n\n");
}

void printStatus(){
  static uint64_t printCounter = 0;
  static uint64_t lastPrint = 0;
  #define PRINT_INTERVAL_MS 333
  printCounter = time_us_64();
  bool shouldPrint = printCounter - lastPrint > PRINT_INTERVAL_MS * 1000;
  // shouldPrint = true;
  if(shouldPrint){
    lastPrint = printCounter;
    printf("RPS\t->\tactual: %.1f\ttarget: %.1f\n", speedController.getActualRPS(), speedController.getTargetRPS());
  }
}

void tick() {
  speedController.tick();
    
  static float rps = 1.5f;
  static constexpr float step = 0.2f;

  if (btn1.hasBeenPressed()) {
    rps -= step;
  }
  if (btn2.hasBeenPressed()) {
    rps +=  step;
  }

  speedController.setTargetRPS(rps);

  printStatus();
}

int main() {
  stdio_init_all();
  sleep_ms(1000);
  
  start();

  while (true) {
    tick();
  }

}
