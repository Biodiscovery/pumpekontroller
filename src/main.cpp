#include <cstdio>
#include <cmath>

#include "pico/stdio.h"
#include "pico/stdlib.h"
/* #include "pico/cyw43_arch.h" */

#include "UI/include/UI/Motor.hpp"
#include "hardware/gpio.h"
#include "board.hpp"
#include "UI/Button.hpp"
#include "UI/PwmPin.hpp"
#include "UI/Motor.hpp"
#include "UI/RotationSpeedController.hpp"

#include "Web/WebServer.hpp"

/* #include "WebServer.hpp"      
 */
using namespace pump_control;

// Hardware buttons
ui::Button btn1(PIN_BTN_1);
ui::Button btn2(PIN_BTN_2);

// Motor driver
ui::Motor motor(PIN_MOTOR_A, PIN_MOTOR_B, 20.f, 50.f);

// Shared power level for both buttons + web interface
static int power = 0;
constexpr int step = 10;

// ----------------------
// Setup
// ----------------------
void start() {
    printf("----- Akselpumpe -----\n");

    btn1.init();
    btn2.init();
    motor.init();
    

    sleep_ms(1000);
    printf("READY.\n\n");
}

// ----------------------
// Apply logic to motor
// ----------------------
void apply_motor_logic() {
    if (power < 0) {
        motor.setDirection(ui::Direction::CLOCKWISE);
    }
    else if (power > 0) {
        motor.setDirection(ui::Direction::COUNTER_CLOCKWISE);
    }
    else {
        motor.setDirection(ui::Direction::OFF);
    }

    motor.setPower(abs(power));
}

// ----------------------
// MAIN
// ----------------------
int main() {
    stdio_init_all();
    sleep_ms(1000);

    start();
    WebServer::init();
    
    
    // --------------------------
    // Main loop
    // --------------------------
    while (true) {
        WebServer::loop();
        // Buttons
        if (btn1.hasBeenPressed()) {
            power -= step;
        }

        if (btn2.hasBeenPressed()) {
            power += step;
        }

        // Clamp?
        if (power > 100) power = 100;
        if (power < -100) power = -100;

        // Apply motor control
        apply_motor_logic();

        // printf("Power: %d\n", power);

        sleep_ms(10);
    }
}
