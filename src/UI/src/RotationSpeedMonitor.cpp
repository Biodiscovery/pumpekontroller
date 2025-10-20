#include <cmath>

#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "hardware/adc.h"

#include "UI/Motor.hpp"
#include "UI/PwmPin.hpp"
#include "Utils/Math.hpp"

#include "UI/RotationSpeedMonitor.hpp"

namespace pump_control::ui {

    void RotationSpeedMonitor::passedMagnet(){
        uint64_t now = time_us_64();
        uint64_t dt  = now - lastPass;
        if (lastPass != 0 && dt > 0) {
            float inst_rps = 1.0f * 1e6f / (static_cast<float>(dt) * divider);
            rps = (rps == 0.0f) ? inst_rps : 0.5f * (rps + inst_rps);
        }
        lastPass = now;
    }

    RotationSpeedMonitor::RotationSpeedMonitor(int pin, int divider) : pin(pin), divider(divider) {}

    void RotationSpeedMonitor::init() {
        adc_init();
        adc_gpio_init(pin);
        if(pin != 26 && pin != 27 && pin != 28){
            while(true){
                printf("!!!EXTREME FAILURE!!! Only pins GPIO26, GPIO27 and GPIO28 can be used for ADC.\n");
            }
        }

        if (pin == 26) adc_select_input(0); else if (pin == 27) adc_select_input(1); else adc_select_input(2);

        // Seed EMA with initial values. Only makes a difference if motor is already spinning.
        uint32_t acc = 0;
        const int seedN = 32;
        for (int i = 0; i < seedN; ++i) {
            acc += adc_read();
            sleep_ms(1);
        }
        float seed = static_cast<float>(acc / seedN);
        emaFast = seed;
        emaSlow = seed;
        armed    = true;

        lastPass = time_us_64();

    }

    static constexpr int   kSlopeThresh      = 600;     // ADC-sum slope threshold to ignore noise
    static constexpr uint32_t kMinDtUs       = 3'000;   // reject events within 2 ms (~30k RPM per 1 pulse/rev)
    static constexpr int   kSamplesPerTick = 8;         // small batch per tick
    static constexpr float kAlphaFast      = 0.20f;     // fast EMA (~responsive)
    static constexpr float kAlphaSlow      = 0.02f;     // slow EMA (baseline drift)
    static constexpr float kThresh         = 25.0f;     // ADC counts above baseline to trigger
    static constexpr float kHyst           = 16.0f;     // hysteresis to re-arm
    
    void RotationSpeedMonitor::tick() {
        if (pin == 26) adc_select_input(0); else if (pin == 27) adc_select_input(1); else adc_select_input(2);

        // Update EMAs with a few fresh samples this tick
        for (int i = 0; i < kSamplesPerTick; ++i) {
            float x = static_cast<float>(adc_read());
            emaFast += kAlphaFast * (x - emaFast);
            emaSlow += kAlphaSlow * (x - emaSlow);
            sleep_ms(1); // keep if your sampling budget allows; otherwise shorten
        }

        // Dynamic thresholds relative to the slow baseline
        float highThresh = emaSlow + kThresh;       // fire when crossing above this
        float lowThresh  = emaSlow + (kThresh - kHyst); // must fall below this to re-arm

        // Schmitt logic (rising-only events)
        uint64_t now = time_us_64();
        bool spacedOut = (now - lastPass) >= kMinDtUs;

        if (armed && emaFast >= highThresh && spacedOut) {
            passedMagnet();   // updates rps & lastPass
            armed = false;    // disarm until we drop below lowThresh
        }
        if (!armed && emaFast <= lowThresh) {
            armed = true;     // re-arm once we've clearly left the peak region
        }

    }

    float RotationSpeedMonitor::getRps() const{
        return rps;
    }

} // namespace pump_control::ui
