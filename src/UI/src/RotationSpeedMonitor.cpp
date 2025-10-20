#include <cmath>

#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "hardware/adc.h"

#include "UI/Motor.hpp"
#include "UI/PwmPin.hpp"
#include "Utils/Math.hpp"

#include "UI/RotationSpeedMonitor.hpp"

namespace pump_control::ui {

    inline bool isPositive(int n){
        return n > 0;
    }

    void RotationSpeedMonitor::Buffer::write(uint16_t raw){
        *writePtr = raw;
        if(++writePtr - rawValues >= NUM_VALUES){
            writePtr = rawValues;
        }
    }

    void RotationSpeedMonitor::Buffer::measure(int n){
        for(int i = 0; i < n; i++){
            write(adc_read());
            sleep_ms(1);
        }
    }


    int RotationSpeedMonitor::Buffer::sum() {
        int s = 0;
        for(int i = 0; i < NUM_VALUES; i++){
            s += rawValues[i];
        }
        return s;
    }

    void RotationSpeedMonitor::passedMagnet(){
        uint64_t now = time_us_64();
        uint64_t dt  = now - lastPass;                 // us
        if (lastPass != 0 && dt > 0) {
            float inst_rpm = 1.0f * 1e6f / (static_cast<float>(dt) * divider);
            // light smoothing; consider EMA if you prefer
            rpm = (rpm == 0.0f) ? inst_rpm : 0.5f * (rpm + inst_rpm);
        }
        lastPass = now;
    }

    RotationSpeedMonitor::RotationSpeedMonitor(int pin, int divider) : pin(pin), divider(divider) {}

    void RotationSpeedMonitor::init() {
        adc_init();
        adc_gpio_init(pin);
        if(pin != 26 && pin != 27){
            while(true){
                printf("!!!EXTREME FAILURE!!! Only pins GPIO26 and GPIO27 can be used for ADC.\n");
            }
        }
        buffer.measure();    // make sure header gives a default or pass an explicit value
        lastSum  = buffer.sum();
        lastDiff = 0;
        lastPass = time_us_64();             // avoids a massive dt on the very first edge
        rpm      = 0.0f;
    }

    // Tunables (adjust to taste)
    static constexpr int   kSlopeThresh      = 600;      // ADC-sum slope threshold to ignore noise
    static constexpr uint32_t kMinDtUs       = 3'000;     // reject events within 2 ms (~30k RPM per 1 pulse/rev)
    
    void RotationSpeedMonitor::tick() {
        if (pin == 26) adc_select_input(0); else adc_select_input(1);

        buffer.measure();

        int newSum = buffer.sum();
        int diff   = lastSum - newSum;          // "slope" proxy (note: negative while rising)
        uint64_t now = time_us_64();

        // Schmitt-like edge detection on the derivative:
        bool risingCross = (lastDiff < 0) && (diff > 0);    // pass near the peak
        bool bigEnough   = (std::abs(diff) > kSlopeThresh); // <-- slope magnitude, not delta slope
        bool spacedOut   = (now - lastPass) >= kMinDtUs;    // as you already had

        // Only fire when "armed"; re-arm after seeing enough negative slope again.
        static bool armed = true;   // or make it a member and init it
        if (armed && risingCross && bigEnough && spacedOut) {
            passedMagnet();
            armed = false;          // disarm until we see strong negative slope again
        }
        if (diff < -kSlopeThresh) {
            armed = true;           // re-arm once we're clearly on the rising flank again
        }

        // (debug printing)
        // printf("newSum: %d\tdiff: %d\tlastSum: %d\tlastDiff: %d\trc: %d\tbe: %d\tso: %d\tarmed:%d\n",
        //     newSum, diff, lastSum, lastDiff, risingCross, bigEnough, spacedOut, armed);

        lastDiff = diff;
        lastSum  = newSum;
        sleep_ms(20);
    }

    float RotationSpeedMonitor::getRpm(){
        return rpm;
    }

} // namespace pump_control::ui
