#pragma once

namespace pump_control::ui {

struct RgbColor {
    bool r = false;
    bool g = false;
    bool b = false;
};

namespace colors{   
static const RgbColor off{false, false, false};
static const RgbColor red{true, false, false};
static const RgbColor green{false, true, false};
static const RgbColor blue{false, false, true};
static const RgbColor cyan{false, true, true};
static const RgbColor yellow{true, true, false};
static const RgbColor magenta{true, false, true};
static const RgbColor white{true, true, true};
}   // namespace colors


class RgbLed {
    private:
    int pinR;
    int pinG;
    int pinB;
    
    public:
    RgbLed(int pinR, int pinG, int pinB);
    void init();
    void setColor(const RgbColor& color);
};

}   //namepsace ui 
