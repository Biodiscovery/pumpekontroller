#pragma once

#include "pico/stdlib.h"

#define RotationSpeedMonitor_PWM_FREQ 50'000

namespace pump_control::ui {
    
    class RotationSpeedMonitor {

        private:
            int pin;
            int divider;

            struct Buffer {
                #define NUM_VALUES 8

                volatile uint16_t rawValues[NUM_VALUES];
                volatile uint16_t *readPtr = rawValues;
                volatile uint16_t *writePtr = rawValues;

                void measure(int n = NUM_VALUES);
                void write(uint16_t raw);
                int sum();
            } buffer;

            int lastSum = 0;
            int lastDiff = 0;

            uint64_t lastPass = 0;
            float rpm = 0.f;

            bool armed = false;

            void passedMagnet();


        public:
            RotationSpeedMonitor(int pin, int divider);
            void init();
            void tick();
            float getRpm();
};

} // namespace pump_control:ui
