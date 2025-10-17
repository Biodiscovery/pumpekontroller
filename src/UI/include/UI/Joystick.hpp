#pragma once

#include "UI/Button.hpp"

namespace pump_control::ui {

class Joystick {
    private:
    int pinX;
    int pinY;
    Button btn;
    
    public:
    Joystick(int pinX, int pinY, int pinBtn);
    void init();
    bool buttonPressed();
    float getX();
    float getY();
};

}   //namepsace ui 
