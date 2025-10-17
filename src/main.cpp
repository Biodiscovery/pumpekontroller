#include <cstdio>
// #include <cstring>

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

  motor.setPower(0);
  
  while (true) {
    static uint64_t printCounter = 0;
    static uint64_t lastPrint = 0;
    #define PRINT_INTERVAL_MS 333

    printCounter = time_us_64();
    bool shouldPrint = printCounter - lastPrint > PRINT_INTERVAL_MS * 1000;
    // shouldPrint = true;
    if(shouldPrint){
      lastPrint = printCounter;
    }

    // tick();
    
    static bool cw = true;
    if(cw){
      motor.setDirection(pump_control::ui::Direction::CLOCKWISE);
    }
    else{
      motor.setDirection(pump_control::ui::Direction::COUNTER_CLOCKWISE);
    }

    for(int i = 0; i < 100; i++){
      motor.setPower(i);
      sleep_ms(10);
    }

    for(int i = 0; i < 100; i++){
      motor.setPower(100 - i);
      sleep_ms(10);
    }
    cw = !cw;

  }
}
