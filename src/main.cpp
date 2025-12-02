
#include "PumpController/PumpController.hpp"



#include <cstdio>
#include <cmath>

#include "pico/stdio.h"
#include "pico/stdlib.h"

#include "UI/include/UI/Motor.hpp"
#include "hardware/gpio.h"
#include "board.hpp"
#include "UI/Button.hpp"
#include "UI/PwmPin.hpp"
#include "UI/Motor.hpp"

#include "Web/WebServer.hpp"



using namespace pump_control;

ui::Button btn1(PIN_BTN_1);
ui::Button btn2(PIN_BTN_2);
ui::Motor motor(PIN_MOTOR_A, PIN_MOTOR_B, 20.f, 50.f);
pump_control::PumpController controller(motor);


// ----------------------
// MAIN
// ----------------------
int main() {
    stdio_init_all();
    sleep_ms(1000);

    btn1.init();
    btn2.init();
    motor.init();

    WebServer::init(controller);

    while (true) {
        WebServer::loop();

        if (btn1.hasBeenPressed()) controller.down();
        if (btn2.hasBeenPressed()) controller.up();

        controller.update();

        sleep_ms(10);
    }
}

