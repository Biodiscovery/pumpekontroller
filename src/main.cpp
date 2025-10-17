#include <cstdio>
#include <cmath>
// #include <cstring>

#include "hardware/gpio.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"

#include "board.hpp"
#include "UI/Button.hpp"
// #include "PumpController/PumpController.hpp"
#include "UI/Joystick.hpp"
// #include "UI/RgbLed.hpp"
// #include "UI/Display.hpp"
#include "UI/PwmPin.hpp"

// #define RX_BUF_SIZE 4
// #define MSG_BUF_SIZE 64

using namespace pump_control;

// PumpController PumpController;

// ui::Display display(PIN_SDA, PIN_SCL);
// ui::RgbLed rgbLed(PIN_RGB1_R, PIN_RGB1_G, PIN_RGB1_B);
// ui::Button btn1(PIN_BTN_1);
// ui::Button btn2(PIN_BTN_2);
ui::Joystick joystick(PIN_JOYSTICK_X, PIN_JOYSTICK_Y, PIN_JOYSTICK_SW);
ui::PwmPin pwmPinA(14, 50'000);
ui::PwmPin pwmPinB(15, 50'000);

// volatile char rxChar = '\0';
// char msgBuf[MSG_BUF_SIZE] = {0};
// char *messagePtr = msgBuf;
// bool messageReceived = false;

// void char_available_callback(void *param) {
//   volatile char *c = (char *)param;
//   *c = stdio_getchar_timeout_us(0);
//   stdio_putchar(*c);
// }

// void resetMsgBuffer() {
//   memset(msgBuf, 0, sizeof(msgBuf));
//   messagePtr = msgBuf;
// }

// bool serialReceive() {
//   char receivedChar = rxChar;
//   rxChar = '\0';
//   if (receivedChar == '\0') {
//     return false;
//   }
//   if (receivedChar == '\n' || receivedChar == '\r') {
//     return true;
//   }
//   if (receivedChar == 0x08 || receivedChar == 0x7F) {
//     if (messagePtr > msgBuf) {
//       messagePtr--;
//       *messagePtr = '\0';
//       printf("\b \b");
//     }
//     return false;
//   }
//   *messagePtr = receivedChar;
//   if (++messagePtr >= msgBuf + sizeof(msgBuf)) {
//     messagePtr = msgBuf;
//   }
//   return false;
// }

void start() {
  printf("----- GlucoSet Fake Sensor -----\n");
  // PumpController.init();

  // rgbLed.init();
  // btn1.init();
  // btn2.init();
  joystick.init();
  // display.init();

  pwmPinA.init();
  pwmPinB.init();

  sleep_ms(1000);
  // stdio_set_chars_available_callback(char_available_callback, (void *)&rxChar);
  printf("READY.\n\n");
}

void tick() {
  // if (serialReceive()) {
  //   if (msgBuf[0] != '\0') {
  //     // MESSAGE RECEIVED
  //   } else {
  //     // NO MESSAGE RECEIVED
  //   }
  //   resetMsgBuffer();
  // }
  // PumpController.tick();
}

int main() {
  stdio_init_all();
  sleep_ms(1000);
  
  start();
  
  while (true) {
  // }
    float joystickX = joystick.getX();
    float joystickY = joystick.getY();

    static bool constantOnMode = true;
    static bool lastConstantOnMode = true;

    if(joystick.buttonPressed()){
      constantOnMode = !constantOnMode;
    }

    static uint64_t printCounter = 0;
    static uint64_t lastPrint = 0;
    #define PRINT_INTERVAL_MS 333

    printCounter = time_us_64();
    bool shouldPrint = printCounter - lastPrint > PRINT_INTERVAL_MS * 1000;
    // shouldPrint = true;
    if(shouldPrint){
      lastPrint = printCounter;
    }

    if(constantOnMode){
        pwmPinB.setDutyCycle(0);
        pwmPinA.setDutyCycle(20.f);
        if(shouldPrint && constantOnMode != lastConstantOnMode){
          printf("Constant on...\n");
        }
    }
    else{
      #define DEAD_ZONE 3.f
      float jsVal = fabs(joystickX);
      // jsVal = copysignf(powf(fabs(jsVal), 1.5f), jsVal);  // Apply smoothing curve for softer low speed control.
      float duty = (jsVal) * 2;
      if(shouldPrint){
        // printf("X: %f\tY: %f\t\tDuty: %f\n", joystickX, joystickY, duty);
        printf("X: %f\t\tDuty: %f\n", joystickX, duty / 100.f);
        // printf("AKSÆÆÆÆÆÆL\n");
      }

      if(joystickX < -DEAD_ZONE){
        pwmPinA.setDutyCycle(0);
        pwmPinB.setDutyCycle(duty);
      }
      else if(joystickX > DEAD_ZONE){
        pwmPinB.setDutyCycle(0);
        pwmPinA.setDutyCycle(duty);

      }
      else{
        pwmPinA.setDutyCycle(0);
        pwmPinB.setDutyCycle(0);
      }
    }
    lastConstantOnMode = constantOnMode;
    sleep_ms(10);
  }
}
